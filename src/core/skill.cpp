#include "skill.h"
#include "engine.h"
#include "player.h"
#include "room.h"
#include "client.h"
#include "standard.h"
#include "scenario.h"
#include "settings.h"

#include <QFile>

Skill::Skill(const QString &name, Frequency frequency)
    :frequency(frequency), default_choice("no")
{
    static QChar lord_symbol('$');

    if(name.endsWith(lord_symbol)){
        QString copy = name;
        copy.remove(lord_symbol);
        setObjectName(copy);
        lord_skill = true;
    }else{
        setObjectName(name);
        lord_skill = false;
    }
}

bool Skill::isLordSkill() const{
    return lord_skill;
}

QString Skill::getDescription() const{
    return Sanguosha->translate(":" + objectName());
}

QString Skill::getText() const{
    QString skill_name = Sanguosha->translate(objectName());

    switch(frequency){
    case Skill::NotFrequent:
    case Skill::Frequent: break;
    case Skill::Limited: skill_name.append(tr(" [Limited]")); break;
    case Skill::Compulsory: skill_name.append(tr(" [Compulsory]")); break;
    case Skill::Wake: skill_name.append(tr(" [Wake]")); break;
    }

    return skill_name;
}

bool Skill::isVisible() const{
    return ! objectName().startsWith("#");
}

QString Skill::getDefaultChoice(ServerPlayer *) const{
    return default_choice;
}

int Skill::getEffectIndex(const ServerPlayer *, const Card *) const{
    return -1;
}

void Skill::initMediaSource(){
    sources.clear();

    int i;
    for(i=1; ;i++){
        QString effect_file = QString("audio/%3/skill/%1%2.ogg").arg(objectName()).arg(i).arg(Config.SoundEffectMode);
        if(QFile::exists(effect_file))
            sources << effect_file;
        else
            break;
    }

    if(sources.isEmpty()){
        QString effect_file = QString("audio/%2/skill/%1.ogg").arg(objectName()).arg(Config.SoundEffectMode);
        if(QFile::exists(effect_file))
            sources << effect_file;
    }
}

Skill::Location Skill::getLocation() const{
    return parent() ? Right : Left;
}

void Skill::playEffect(int index) const{
    if(!sources.isEmpty()){
        if(index == -1)
            index = qrand() % sources.length();
        else
            index--;

        // check length
        QString filename;
        if(index >= 0 && index < sources.length())
            filename = sources.at(index);
        else
            filename = sources.first();

        Sanguosha->playEffect(filename);
        if(ClientInstance)
            ClientInstance->setLines(filename);
    }
}

void Skill::setFlag(ServerPlayer *player) const{
    player->getRoom()->setPlayerFlag(player, objectName());
}

void Skill::unsetFlag(ServerPlayer *player) const{
    player->getRoom()->setPlayerFlag(player, "-" + objectName());
}

Skill::Frequency Skill::getFrequency() const{
    return frequency;
}

QStringList Skill::getSources() const{
    return sources;
}

QDialog *Skill::getDialog() const{
    return NULL;
}

ViewAsSkill::ViewAsSkill(const QString &name)
    :Skill(name)
{

}

bool ViewAsSkill::isAvailable() const{
    switch(ClientInstance->getStatus()){
    case Client::Playing: return isEnabledAtPlay(Self) && (!Self->loseViewAsSkills()
                                                           || this->objectName() == "free-discard"
                                                           || this->objectName() == "fan"
                                                           || this->objectName() == "spear"
                                                           || this->objectName() == "huangtianv"
                                                           || this->objectName() == "zhiba_pindian"
                                                           || this->objectName() == "liangshangjunzi"
                                                           || this->objectName() == "peaching");
    case Client::Responsing: return isEnabledAtResponse(Self, ClientInstance->getPattern())
                && (!Self->loseViewAsSkills()
                    || this->objectName() == "wuliu_sword");
    default:
        return false;
    }
}

bool ViewAsSkill::isEnabledAtPlay(const Player *) const{
    return true;
}

bool ViewAsSkill::isEnabledAtResponse(const Player *, const QString &) const{
    return false;
}

ZeroCardViewAsSkill::ZeroCardViewAsSkill(const QString &name)
    :ViewAsSkill(name)
{

}

const Card *ZeroCardViewAsSkill::viewAs(const QList<CardItem *> &cards) const{
    if(cards.isEmpty())
        return viewAs();
    else
        return NULL;
}

bool ZeroCardViewAsSkill::viewFilter(const QList<CardItem *> &, const CardItem *) const{
    return false;
}

OneCardViewAsSkill::OneCardViewAsSkill(const QString &name)
    :ViewAsSkill(name)
{

}

bool OneCardViewAsSkill::viewFilter(const QList<CardItem *> &selected, const CardItem *to_select) const{
    return selected.isEmpty() && viewFilter(to_select);
}

const Card *OneCardViewAsSkill::viewAs(const QList<CardItem *> &cards) const{
    if(cards.length() != 1)
        return NULL;
    else
        return viewAs(cards.first());
}

FilterSkill::FilterSkill(const QString &name)
    :OneCardViewAsSkill(name)
{
    frequency = Compulsory;
}

TriggerSkill::TriggerSkill(const QString &name)
    :Skill(name), view_as_skill(NULL)
{

}

const ViewAsSkill *TriggerSkill::getViewAsSkill() const{
    return view_as_skill;
}

QList<TriggerEvent> TriggerSkill::getTriggerEvents() const{
    return events;
}

int TriggerSkill::getPriority() const{
    switch(frequency){
    case Compulsory:
    case Wake: return 2;
    default:
        return 1;
    }
}

int TriggerSkill::secondPriority() const{
    return 1;
}

bool TriggerSkill::triggerable(const ServerPlayer *target) const{
    return target != NULL && target->isAlive() && target->hasSkill(objectName());
}

ScenarioRule::ScenarioRule(Scenario *scenario)
    :TriggerSkill(scenario->objectName())
{
    setParent(scenario);
}

int ScenarioRule::getPriority() const{
    return 3;
}

bool ScenarioRule::triggerable(const ServerPlayer *) const{
    return true;
}

MasochismSkill::MasochismSkill(const QString &name)
    :TriggerSkill(name)
{
    events << Damaged;
}

int MasochismSkill::getPriority() const{
    return -1;
}

bool MasochismSkill::trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &data) const{
    DamageStruct damage = data.value<DamageStruct>();

    if(player->isAlive())
        onDamaged(player, damage);

    return false;
}

PhaseChangeSkill::PhaseChangeSkill(const QString &name)
    :TriggerSkill(name)
{
    events << PhaseChange;
}

bool PhaseChangeSkill::trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &) const{
    bool skipped = onPhaseChange(player);
    if(skipped)
        player->skip(player->getPhase());

    return skipped;
}

DrawCardsSkill::DrawCardsSkill(const QString &name)
    :TriggerSkill(name)
{
    events << DrawNCards;
}

bool DrawCardsSkill::trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &data) const{
    int n = data.toInt();
    data = getDrawNum(player, n);
    return false;
}

SlashBuffSkill::SlashBuffSkill(const QString &name)
    :TriggerSkill(name)
{
    events << SlashProceed;
}

bool SlashBuffSkill::trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &data) const{
    if(data.canConvert<SlashEffectStruct>()){
        SlashEffectStruct effect = data.value<SlashEffectStruct>();

        if(player->isAlive())
            return buff(effect);
    }

    return false;
}

GameStartSkill::GameStartSkill(const QString &name)
    :TriggerSkill(name)
{
    events << GameStart;
}

bool GameStartSkill::trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &) const{
    onGameStart(player);
    return false;
}

SPConvertSkill::SPConvertSkill(const QString &name, const QString &from, const QString &to)
    :GameStartSkill(name), from(from), to(to)
{
    frequency = Limited;
}

bool SPConvertSkill::triggerable(const ServerPlayer *target) const{
    if (target == NULL) return false;
    bool canInvoke = ServerInfo.GameMode.endsWith("p") || ServerInfo.GameMode.endsWith("pd") ||
            ServerInfo.GameMode.endsWith("pz");
    QString package = Sanguosha->getGeneral(to)->getPackage();
    if(Sanguosha->getBanPackages().contains(package)) return false;
    return GameStartSkill::triggerable(target) && target->getGeneralName() == from && canInvoke;
}

void SPConvertSkill::onGameStart(ServerPlayer *player) const{
    if(player->askForSkillInvoke(objectName())){
        Room *room = player->getRoom();
        room->setPlayerProperty(player, "general", to);

        const General *general = Sanguosha->getGeneral(to);
        const QString kingdom = general->getKingdom();
        if(kingdom != player->getKingdom())
            room->setPlayerProperty(player, "kingdom", kingdom);

    }
}

TransfigureSkill::TransfigureSkill(const QString &name, const QString &from, const QString &to, const QString &mark_willlose)
    :GameStartSkill(name), from(from), to(to), mark_willlose(mark_willlose)
{
    frequency = Limited;
}

bool TransfigureSkill::triggerable(const ServerPlayer *target) const{
    if (target == NULL) return false;
    if(ServerInfo.Enable2ndGeneral)
    {
        QStringList ban_list = Config.value("Banlist/Pairs").toStringList();
        QString ban1(QString("%1+%2").arg(to).arg(target->getGeneral2Name()));
        QString ban2(QString("%1+%2").arg(target->getGeneral2Name()).arg(to));
        if(ban_list.contains(ban1) || ban_list.contains(ban2) || ban_list.contains(to))
            return false;
    }
    bool canInvoke = ServerInfo.GameMode.endsWith("p") || ServerInfo.GameMode.endsWith("pd") ||
            ServerInfo.GameMode.endsWith("pz");
	const General *to_general = Sanguosha->getGeneral(to);
	if(to_general == NULL || Sanguosha->getBanPackages().contains(to_general->getPackage()))
		return false;
    return GameStartSkill::triggerable(target) && target->getGeneralName() == from && canInvoke;
}

void TransfigureSkill::onGameStart(ServerPlayer *player) const{
    if(player->askForSkillInvoke(objectName())){
        Room *room = player->getRoom();

        if(!mark_willlose.isEmpty())
            player->loseAllMarks(mark_willlose);

        room->transfigure(player, to, true);

        const General *general = Sanguosha->getGeneral(to);
        const QString kingdom = general->getKingdom();
        if(kingdom != player->getKingdom() && kingdom != "god")
            room->setPlayerProperty(player, "kingdom", kingdom);
    }
}

ProhibitSkill::ProhibitSkill(const QString &name)
    :Skill(name, Skill::Compulsory)
{
}

DistanceSkill::DistanceSkill(const QString &name)
    :Skill(name, Skill::Compulsory)
{

}

MaxCardsSkill::MaxCardsSkill(const QString &name)
    :Skill(name, Skill::Compulsory)
{

}

WeaponSkill::WeaponSkill(const QString &name)
    :TriggerSkill(name)
{
}

bool WeaponSkill::triggerable(const ServerPlayer *target) const{
    if (target == NULL) return false;
    return target->hasWeapon(objectName());
}

int WeaponSkill::secondPriority() const{
    return 0;
}

ArmorSkill::ArmorSkill(const QString &name)
    :TriggerSkill(name)
{

}

bool ArmorSkill::triggerable(const ServerPlayer *target) const{
    if (target == NULL) return false;
    return target->hasArmorEffect(objectName()) && target->getArmor()->getSkill() == this;
}

int ArmorSkill::secondPriority() const{
    return 0;
}

MarkAssignSkill::MarkAssignSkill(const QString &mark, int n)
    :GameStartSkill(QString("#%1-%2").arg(mark).arg(n)), mark_name(mark), n(n)
{
}

int MarkAssignSkill::getPriority() const{
    return 3;
}

void MarkAssignSkill::onGameStart(ServerPlayer *player) const{
    player->getRoom()->setPlayerMark(player, mark_name, n);
}
