#include "god.h"
#include "client.h"
#include "engine.h"
#include "carditem.h"
#include "settings.h"
#include "maneuvering.h"
#include "general.h"

#include <QFile>

GongxinCard::GongxinCard(){
    once = true;
}

bool GongxinCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const{
    return targets.isEmpty() && !to_select->isKongcheng() && to_select != Self ;
}

void GongxinCard::onEffect(const CardEffectStruct &effect) const{
    effect.from->getRoom()->doGongxin(effect.from, effect.to);
}

class Wuhun: public TriggerSkill{
public:
    Wuhun():TriggerSkill("wuhun"){
        events << Damaged;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent, Room*, ServerPlayer *player, QVariant &data) const{
        DamageStruct damage = data.value<DamageStruct>();

        if(damage.from && damage.from != player){
            damage.from->gainMark("@nightmare", damage.damage);
            damage.from->getRoom()->playSkillEffect(objectName(), 1);
        }

        return false;
    }
};

class WuhunRevenge: public TriggerSkill{
public:
    WuhunRevenge():TriggerSkill("#wuhun"){
        events << Death;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return target->hasSkill("wuhun");
    }

    virtual bool trigger(TriggerEvent, Room* room, ServerPlayer *shenguanyu, QVariant &) const{
        QList<ServerPlayer *> players = room->getOtherPlayers(shenguanyu);

        int max = 0;
        foreach(ServerPlayer *player, players){
            max = qMax(max, player->getMark("@nightmare"));
        }

        if(max == 0)
            return false;

        QList<ServerPlayer *> foes;
        foreach(ServerPlayer *player, players){
            if(player->getMark("@nightmare") == max)
                foes << player;
        }

        if(foes.isEmpty())
            return false;

        ServerPlayer *foe;
        if(foes.length() == 1)
            foe = foes.first();
        else
            foe = room->askForPlayerChosen(shenguanyu, foes, "wuhun");

        JudgeStruct judge;
        judge.pattern = QRegExp("(Peach|GodSalvation):(.*):(.*)");
        judge.good = true;
        judge.reason = "wuhun";
        judge.who = foe;

        room->judge(judge);

        if(judge.isBad()){
            LogMessage log;
            log.type = "#WuhunRevenge";
            log.from = shenguanyu;
            log.to << foe;
            log.arg = QString::number(max);
            log.arg2 = "wuhun";
            room->sendLog(log);

            room->killPlayer(foe);
            room->playSkillEffect("wuhun", 2);
        }else
            room->playSkillEffect("wuhun", 3);
        QList<ServerPlayer *> killers = room->getAllPlayers();
        foreach(ServerPlayer *player, killers){
            player->loseAllMarks("@nightmare");
        }
        return false;
    }
};

static bool CompareBySuit(int card1, int card2){
    const Card *c1 = Sanguosha->getCard(card1);
    const Card *c2 = Sanguosha->getCard(card2);

    int a = static_cast<int>(c1->getSuit());
    int b = static_cast<int>(c2->getSuit());

    return a < b;
}

class Shelie: public PhaseChangeSkill{
public:
    Shelie():PhaseChangeSkill("shelie"){

    }

    virtual bool onPhaseChange(ServerPlayer *shenlvmeng) const{
        if(shenlvmeng->getPhase() != Player::Draw)
            return false;

        Room *room = shenlvmeng->getRoom();
        if(!shenlvmeng->askForSkillInvoke(objectName()))
            return false;

        room->playSkillEffect(objectName());

        QList<int> card_ids = room->getNCards(5);
        qSort(card_ids.begin(), card_ids.end(), CompareBySuit);
        room->fillAG(card_ids);

        while(!card_ids.isEmpty()){
            int card_id = room->askForAG(shenlvmeng, card_ids, false, "shelie");
            card_ids.removeOne(card_id);
            room->takeAG(shenlvmeng, card_id);

            // throw the rest cards that matches the same suit
            const Card *card = Sanguosha->getCard(card_id);
            Card::Suit suit = card->getSuit();
            QMutableListIterator<int> itor(card_ids);
            while(itor.hasNext()){
                const Card *c = Sanguosha->getCard(itor.next());
                if(c->getSuit() == suit){
                    itor.remove();

                    room->takeAG(NULL, c->getId());
                }
            }
        }

        room->broadcastInvoke("clearAG");

        return true;
    }
};

class Gongxin: public ZeroCardViewAsSkill{
public:
    Gongxin():ZeroCardViewAsSkill("gongxin"){
        default_choice = "discard";
    }

    virtual const Card *viewAs() const{
        return new GongxinCard;
    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return !player->hasUsed("GongxinCard");
    }
};

void YeyanCard::damage(ServerPlayer *shenzhouyu, ServerPlayer *target, int point) const{
    DamageStruct damage;

    damage.card = NULL;
    damage.from = shenzhouyu;
    damage.to = target;
    damage.damage = point;
    damage.nature = DamageStruct::Fire;

    shenzhouyu->getRoom()->damage(damage);
}

GreatYeyanCard::GreatYeyanCard(){

}

bool GreatYeyanCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const{
    return targets.isEmpty();
}

void GreatYeyanCard::use(Room *room, ServerPlayer *shenzhouyu, const QList<ServerPlayer *> &targets) const{
    room->broadcastInvoke("animate", "lightbox:$greatyeyan");

    shenzhouyu->loseMark("@flame");
    room->throwCard(this, shenzhouyu);
    room->loseHp(shenzhouyu, 3);

    damage(shenzhouyu, targets.first(), 3);
}

MediumYeyanCard::MediumYeyanCard(){

}

bool MediumYeyanCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const{
    return targets.length() < 2;
}

void MediumYeyanCard::use(Room *room, ServerPlayer *shenzhouyu, const QList<ServerPlayer *> &targets) const{
    room->broadcastInvoke("animate", "lightbox:$mediumyeyan");

    shenzhouyu->loseMark("@flame");
    room->throwCard(this, shenzhouyu);
    room->loseHp(shenzhouyu, 3);

    ServerPlayer *first = targets.first();
    ServerPlayer *second = targets.value(1, NULL);

    damage(shenzhouyu, first, 2);

    if(second)
        damage(shenzhouyu, second, 1);
}

SmallYeyanCard::SmallYeyanCard(){

}

bool SmallYeyanCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const{
    return targets.length() < 3;
}

void SmallYeyanCard::use(Room *room, ServerPlayer *shenzhouyu, const QList<ServerPlayer *> &targets) const{
    room->broadcastInvoke("animate", "lightbox:$smallyeyan");
    shenzhouyu->loseMark("@flame");

    Card::use(room, shenzhouyu, targets);
}

void SmallYeyanCard::onEffect(const CardEffectStruct &effect) const{
    damage(effect.from, effect.to, 1);
}

class GreatYeyan: public ViewAsSkill{
public:
    GreatYeyan(): ViewAsSkill("greatyeyan"){
        frequency = Limited;
    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return player->getMark("@flame") >= 1;
    }

    virtual bool viewFilter(const QList<CardItem *> &selected, const CardItem *to_select) const{
        if(selected.length() >= 4)
            return false;

        if(to_select->isEquipped())
            return false;

        foreach(CardItem *item, selected){
            if(to_select->getFilteredCard()->getSuit() == item->getFilteredCard()->getSuit())
                return false;
        }

        return true;
    }

    virtual const Card *viewAs(const QList<CardItem *> &cards) const{
        if(cards.length() != 4)
            return NULL;

        GreatYeyanCard *card = new GreatYeyanCard;
        card->addSubcards(cards);

        return card;
    }
};

class MediumYeyan: public GreatYeyan{
public:
    MediumYeyan(){
        setObjectName("mediumyeyan");
    }

    virtual const Card *viewAs(const QList<CardItem *> &cards) const{
        if(cards.length() != 4)
            return NULL;

        MediumYeyanCard *card = new MediumYeyanCard;
        card->addSubcards(cards);

        return card;
    }
};

class SmallYeyan: public ZeroCardViewAsSkill{
public:
    SmallYeyan():ZeroCardViewAsSkill("smallyeyan"){
        frequency = Limited;
    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return player->getMark("@flame") >= 1;
    }

    virtual const Card *viewAs() const{
        return new SmallYeyanCard;
    }
};

class Qinyin: public TriggerSkill{
public:
    Qinyin():TriggerSkill("qinyin"){
        events << CardLostOnePiece << PhaseChange;
        default_choice = "down";
    }

    void perform(ServerPlayer *shenzhouyu) const{
        Room *room = shenzhouyu->getRoom();
        QString result = room->askForChoice(shenzhouyu, objectName(), "up+down");
        QList<ServerPlayer *> all_players = room->getAllPlayers();
        if(result == "up"){
            room->playSkillEffect(objectName(), 2);
            foreach(ServerPlayer *player, all_players){
                RecoverStruct recover;
                recover.who = shenzhouyu;
                room->recover(player, recover);
            }
        }else if(result == "down"){
            foreach(ServerPlayer *player, all_players){
                room->loseHp(player);
            }

            int index = 1;
            if(room->findPlayer("caocao+shencaocao+weiwudi"))
                index = 3;

            room->playSkillEffect(objectName(), index);
        }
    }

    virtual bool trigger(TriggerEvent event, Room*, ServerPlayer *shenzhouyu, QVariant &data) const{
        if(shenzhouyu->getPhase() != Player::Discard)
            return false;

        if(event == CardLostOnePiece){
            CardMoveStar move = data.value<CardMoveStar>();
            if(move->to_place == Player::DiscardPile){
                shenzhouyu->addMark("qinyin");
                if(shenzhouyu->getMark("qinyin") == 2){
                    if(shenzhouyu->askForSkillInvoke(objectName()))
                        perform(shenzhouyu);
                }
            }
        }else if(event == PhaseChange){
            shenzhouyu->setMark("qinyin", 0);
        }

        return false;
    }
};

class Guixin: public MasochismSkill{
public:
    Guixin():MasochismSkill("guixin"){

    }

    virtual void onDamaged(ServerPlayer *shencc, const DamageStruct &damage) const{
        Room *room = shencc->getRoom();
        int i, x = damage.damage;
        for(i=0; i<x; i++){
            if(shencc->askForSkillInvoke(objectName())){
                room->playSkillEffect(objectName());

                QList<ServerPlayer *> players = room->getOtherPlayers(shencc);
                if(players.length() >=5)
                    room->broadcastInvoke("animate", "lightbox:$guixin");

                foreach(ServerPlayer *player, players){
                    if(!player->isAllNude()){
                        int card_id = room->askForCardChosen(shencc, player, "hej", objectName());
                        room->obtainCard(shencc, card_id, room->getCardPlace(card_id) != Player::Hand);
                    }
                }

                shencc->turnOver();
            }else
                break;
        }
    }
};

class Feiying: public DistanceSkill{
public:
    Feiying():DistanceSkill("feiying"){

    }

    virtual int getCorrect(const Player *from, const Player *to) const{
        if(to->hasSkill(objectName()))
            return +1;
        else
            return 0;
    }
};

class Kuangbao: public TriggerSkill{
public:
    Kuangbao():TriggerSkill("kuangbao"){
        events << Damage << Damaged;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent event, Room* room, ServerPlayer *player, QVariant &data) const{
        DamageStruct damage = data.value<DamageStruct>();

        LogMessage log;
        log.type = event == Damage ? "#KuangbaoDamage" : "#KuangbaoDamaged";
        log.from = player;
        log.arg = QString::number(damage.damage);
        log.arg2 = objectName();
        player->getRoom()->sendLog(log);

        player->gainMark("@wrath", damage.damage);
        room->playSkillEffect(objectName());

        return false;
    }
};

class Wumou:public TriggerSkill{
public:
    Wumou():TriggerSkill("wumou"){
        frequency = Compulsory;
        events << CardUsed << CardResponsed;
    }

    virtual bool trigger(TriggerEvent event, Room* room, ServerPlayer *player, QVariant &data) const{
        CardStar card = NULL;
        if(event == CardUsed){
            CardUseStruct use = data.value<CardUseStruct>();
            card = use.card;
        }else if(event == CardResponsed)
            card = data.value<CardStar>();

        if(card->inherits("TrickCard") && !card->inherits("DelayedTrick")){
            room->playSkillEffect(objectName());

            int num = player->getMark("@wrath");
            if(num >= 1 && room->askForChoice(player, objectName(), "discard+losehp") == "discard"){
                player->loseMark("@wrath");
            }else
                room->loseHp(player);
        }

        return false;
    }
};

class Shenfen:public ZeroCardViewAsSkill{
public:
    Shenfen():ZeroCardViewAsSkill("shenfen"){
    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return player->getMark("@wrath") >= 6 && !player->hasUsed("ShenfenCard");
    }

    virtual const Card *viewAs() const{
        return new ShenfenCard;
    }
};


ShenfenCard::ShenfenCard(){
    target_fixed = true;
    once = true;
}

void ShenfenCard::use(Room *room, ServerPlayer *shenlvbu, const QList<ServerPlayer *> &) const{
    shenlvbu->loseMark("@wrath", 6);

    QList<ServerPlayer *> players = room->getOtherPlayers(shenlvbu);
    foreach(ServerPlayer *player, players){
        DamageStruct damage;
        damage.card = this;
        damage.from = shenlvbu;
        damage.to = player;

        room->damage(damage);
    }

    foreach(ServerPlayer *player, players){
        player->throwAllEquips();
    }

    foreach(ServerPlayer *player, players){
        if(player->getHandcardNum() <= 4)
            player->throwAllHandCards();
        else
            room->askForDiscard(player, "shenfen", 4, 4);
    }

    shenlvbu->turnOver();
}

WuqianCard::WuqianCard(){
    once = true;
}

bool WuqianCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const{
    return targets.isEmpty() && to_select != Self ;
}

void WuqianCard::onEffect(const CardEffectStruct &effect) const{
    Room *room = effect.to->getRoom();

    effect.from->loseMark("@wrath", 2);
    room->acquireSkill(effect.from, "wushuang", false);
    effect.from->setFlags("wuqian_used");

    room->setTag("WuqianTarget", QVariant::fromValue(effect.to));
}

class WuqianViewAsSkill: public ZeroCardViewAsSkill{
public:
    WuqianViewAsSkill():ZeroCardViewAsSkill("wuqian"){

    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return player->getMark("@wrath") >= 2;
    }

    virtual const Card *viewAs() const{
        return new WuqianCard;
    }
};

class Wuqian: public TriggerSkill{
public:
    Wuqian():TriggerSkill("wuqian"){
        events << CardUsed << CardFinished << PhaseChange << Death;
        view_as_skill = new WuqianViewAsSkill;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return true;
    }

    virtual bool trigger(TriggerEvent event, Room* room, ServerPlayer *player, QVariant &data) const{
        ServerPlayer *target = room->getTag("WuqianTarget").value<PlayerStar>();
        if(!target)
            return false;

        if(event == PhaseChange || event == Death){
            if(player->hasSkill(objectName()) && (event == Death || player->getPhase() == Player::NotActive)){
                room->removeTag("WuqianTarget");

                if(!player->hasInnateSkill("wushuang"))
                    player->loseSkill("wushuang");
            }
        }
        else{
            CardUseStruct use = data.value<CardUseStruct>();
            if(use.to.contains(target)){
                if(event == CardUsed)
                    target->addMark("qinggang");
                else
                    target->removeMark("qinggang");
            }
        }

        return false;
    }
};


WushenSlash::WushenSlash(Card::Suit suit, int number)
    :Slash(suit, number)
{

}

class Wushen: public FilterSkill{
public:
    Wushen():FilterSkill("wushen"){

    }

    virtual bool viewFilter(const CardItem *to_select) const{
        return to_select->getCard()->getSuit() == Card::Heart;
    }

    virtual const Card *viewAs(CardItem *card_item) const{
        const Card *card = card_item->getCard();
        WushenSlash *slash = new WushenSlash(card->getSuit(), card->getNumber());
        slash->addSubcard(card_item->getCard()->getId());
        slash->setSkillName(objectName());

        return slash;
    }
};

class Qixing: public PhaseChangeSkill{
public:
    Qixing():PhaseChangeSkill("qixing"){
        frequency = Frequent;
    }

    virtual int getPriority() const{
        return -1;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return PhaseChangeSkill::triggerable(target) && target->getMark("@star") > 0;
    }

    static void Exchange(ServerPlayer *shenzhuge){
        QList<int> stars = shenzhuge->getPile("stars");
        if(stars.isEmpty())
            return;

        Room *room = shenzhuge->getRoom();
        room->playSkillEffect("qixing");

        int ai_delay = Config.AIDelay;
        Config.AIDelay = 0;

        int n = 0;
        while(!stars.isEmpty()){
            room->fillAG(stars, shenzhuge);
            int card_id = room->askForAG(shenzhuge, stars, true, "qixing");
            if(card_id == -1)
            {
                shenzhuge->invoke("clearAG");
                break;
            }

            stars.removeOne(card_id);
            ++n;

            room->obtainCard(shenzhuge, card_id, false);
            shenzhuge->invoke("clearAG");
        }

        Config.AIDelay = ai_delay;


        if(n == 0)
            return;

        const Card *exchange_card = room->askForExchange(shenzhuge, "qixing", n);

        foreach(int card_id, exchange_card->getSubcards())
            shenzhuge->addToPile("stars", card_id, false);

        LogMessage log;
        log.type = "#QixingExchange";
        log.from = shenzhuge;
        log.arg = QString::number(n);
        log.arg2 = "qixing";
        room->sendLog(log);

        delete exchange_card;
    }

    static void DiscardStar(ServerPlayer *shenzhuge, int n, QString skillName){
        Room *room = shenzhuge->getRoom();
        QList<int> stars = shenzhuge->getPile("stars");

        for(int i = 0; i < n; i++){
            room->fillAG(stars, shenzhuge);
            int card_id = room->askForAG(shenzhuge, stars, false, "qixing-discard");
            CardMoveReason reason(CardMoveReason::S_REASON_REMOVE_FROM_PILE, shenzhuge->objectName(), skillName, QString());
            room->throwCard(Sanguosha->getCard(card_id), reason, NULL);
            stars.removeOne(card_id);
            shenzhuge->invoke("clearAG");
        }
    }

    virtual bool onPhaseChange(ServerPlayer *shenzhuge) const{
        if(shenzhuge->getPhase() == Player::Draw){
            Exchange(shenzhuge);
        }

        return false;
    }
};

class QixingStart: public GameStartSkill{
public:
    QixingStart():GameStartSkill("#qixing"){

    }

    virtual int getPriority() const{
        return -1;
    }

    virtual void onGameStart(ServerPlayer *shenzhuge) const{
        shenzhuge->gainMark("@star", 7);
        QList<int> stars;
        for (int i = 0; i < 7; i++)
        {
            stars.push_back(shenzhuge->getRoom()->drawCard());
        }
        shenzhuge->addToPile("stars", stars, false);
        Qixing::Exchange(shenzhuge);
    }
};

KuangfengCard::KuangfengCard(){

}

bool KuangfengCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const{
    return targets.isEmpty();
}

void KuangfengCard::onEffect(const CardEffectStruct &effect) const{
    Qixing::DiscardStar(effect.from, 1, "kuangfeng");

    effect.from->loseMark("@star");
    effect.to->gainMark("@gale");
}

class KuangfengViewAsSkill: public ZeroCardViewAsSkill{
public:
    KuangfengViewAsSkill():ZeroCardViewAsSkill("kuangfeng"){

    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return false;
    }

    virtual bool isEnabledAtResponse(const Player *player, const QString &pattern) const{
        return pattern == "@@kuangfeng";
    }

    virtual const Card *viewAs() const{
        return new KuangfengCard;
    }
};

class Kuangfeng: public TriggerSkill{
public:
    Kuangfeng():TriggerSkill("kuangfeng"){
        view_as_skill = new KuangfengViewAsSkill;

        events << DamagedBegin;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return target->getMark("@gale") > 0;
    }

    virtual bool trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &data) const{
        DamageStruct damage = data.value<DamageStruct>();
        if(damage.nature == DamageStruct::Fire){
            LogMessage log;
            log.type = "#GalePower";
            log.from = player;
            log.arg = QString::number(damage.damage);
            log.arg2 = QString::number(damage.damage + 1);
            room->sendLog(log);

            room->setEmotion(player, QString("skill/%1").arg(objectName()));

            damage.damage ++;
            data = QVariant::fromValue(damage);
        }

        return false;
    }
};

class QixingAsk: public PhaseChangeSkill{
public:
    QixingAsk():PhaseChangeSkill("#qixing-ask"){

    }

    virtual bool onPhaseChange(ServerPlayer *target) const{
        Room *room = target->getRoom();
        if(target->getPhase() == Player::Finish){
            if(target->getMark("@star") > 0 && target->hasSkill("kuangfeng"))
                room->askForUseCard(target, "@@kuangfeng", "@kuangfeng-card");

            if(target->getMark("@star") > 0 && target->hasSkill("dawu"))
                room->askForUseCard(target, "@@dawu", "@dawu-card");
        }else if(target->getPhase() == Player::Start){
            QList<ServerPlayer *> players = room->getAllPlayers();
            foreach(ServerPlayer *player, players){
                if(player->getMark("@gale") > 0)
                    player->loseMark("@gale");
                if(player->getMark("@fog") > 0)
                    player->loseMark("@fog");
            }
        }

        return false;
    }
};

class QixingClear: public TriggerSkill{
public:
    QixingClear():TriggerSkill("#qixing-clear"){
        events << Death;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return target->hasSkill(objectName());
    }

    virtual bool trigger(TriggerEvent , Room* room, ServerPlayer *, QVariant &) const{
        QList<ServerPlayer *> players = room->getAllPlayers();
        foreach(ServerPlayer *player, players){
            player->loseAllMarks("@gale");
            player->loseAllMarks("@fog");
        }

        return false;
    }
};

DawuCard::DawuCard(){

}

bool DawuCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const{
    return targets.length() < Self->getMark("@star");
}

void DawuCard::use(Room *, ServerPlayer *source, const QList<ServerPlayer *> &targets) const{
    int n = targets.length();
    Qixing::DiscardStar(source, n, "dawu");

    source->loseMark("@star", n);
    foreach(ServerPlayer *target, targets){
        target->gainMark("@fog");
    }
}

class DawuViewAsSkill: public ZeroCardViewAsSkill{
public:
    DawuViewAsSkill():ZeroCardViewAsSkill("dawu"){

    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return false;
    }

    virtual bool isEnabledAtResponse(const Player *player, const QString &pattern) const{
        return pattern == "@@dawu";
    }

    virtual const Card *viewAs() const{
        return new DawuCard;
    }
};

class Dawu: public TriggerSkill{
public:
    Dawu():TriggerSkill("dawu"){
        view_as_skill = new DawuViewAsSkill;

        events << DamagedBegin;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return target->getMark("@fog") > 0;
    }

    virtual bool trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &data) const{
        DamageStruct damage = data.value<DamageStruct>();
        if(damage.nature != DamageStruct::Thunder){

            LogMessage log;
            log.type = "#FogProtect";
            log.from = player;
            log.arg = QString::number(damage.damage);
            if(damage.nature == DamageStruct::Normal)
                log.arg2 = "normal_nature";
            else if(damage.nature == DamageStruct::Fire)
                log.arg2 = "fire_nature";
            room->sendLog(log);

            room->setEmotion(player, QString("skill/%1").arg(objectName()));
            return true;
        }else
            return false;
    }
};

class Renjie: public TriggerSkill{
public:
    Renjie():TriggerSkill("renjie"){
        events << Damaged << CardDiscarded;
        frequency = Compulsory;
    }

    virtual bool trigger(TriggerEvent event, Room* room, ServerPlayer *player, QVariant &data) const{
        if(event == CardDiscarded){
            if(player->getPhase() == Player::Discard){
                CardStar card = data.value<CardStar>();
                int n = card->subcardsLength();
                if(n > 0){
                    room->playSkillEffect(objectName());
                    player->gainMark("@bear", n);
                }
            }
        }else if(event == Damaged){
            DamageStruct damage = data.value<DamageStruct>();
            room->playSkillEffect(objectName());
            player->gainMark("@bear", damage.damage);
        }

        return false;
    }
};

class LianpoCount: public TriggerSkill{
public:
    LianpoCount():TriggerSkill("#lianpo-count"){
        events << Death;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return true;
    }

    virtual bool trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &data) const{
        DamageStar damage = data.value<DamageStar>();
        ServerPlayer *killer = damage ? damage->from : NULL;

        if(killer && killer->hasSkill("lianpo")){
            killer->addMark("lianpo");

            LogMessage log;
            log.type = "#LianpoRecord";
            log.from = killer;
            log.to << player;

            log.arg = room->getCurrent()->getGeneralName();
            room->sendLog(log);
        }

        return false;
    }
};

class Baiyin: public PhaseChangeSkill{
public:
    Baiyin():PhaseChangeSkill("baiyin"){
        frequency = Wake;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return PhaseChangeSkill::triggerable(target)
                && target->getPhase() == Player::Start
                && target->getMark("baiyin") == 0
                && target->getMark("@bear") >= 4;
    }

    virtual bool onPhaseChange(ServerPlayer *shensimayi) const{
        Room *room = shensimayi->getRoom();

        LogMessage log;
        log.type = "#BaiyinWake";
        log.from = shensimayi;
        log.arg = QString::number(shensimayi->getMark("@bear"));
        room->sendLog(log);
        room->playSkillEffect(objectName());

        room->loseMaxHp(shensimayi);

        QString effect_file = QString("audio/%1/skill/baiyin.ogg").arg(Config.SoundEffectMode);
        if(QFile::exists(effect_file)){
            room->broadcastInvoke("animate", "lightbox:$baiyin:3300");
            room->getThread()->delay(3300);
        }

        room->acquireSkill(shensimayi, "jilve");

        shensimayi->setMark("baiyin", 1);

        return false;
    }
};

JilveCard::JilveCard(){
    target_fixed = true;
}

void JilveCard::onUse(Room *room, const CardUseStruct &card_use) const{
    ServerPlayer *shensimayi = card_use.from;

    QStringList choices;
    if(!shensimayi->hasUsed("ZhihengCard"))
        choices << "zhiheng";

    if(!shensimayi->tag.value("JilveWansha", false).toBool())
        choices << "wansha";

    if(choices.isEmpty())
        return;

    QString choice;
    if(choices.length() == 1)
        choice = choices.first();
    else
        choice = room->askForChoice(shensimayi, "jilve", "zhiheng+wansha");

    shensimayi->loseMark("@bear");

    if(choice == "wansha"){
        room->acquireSkill(shensimayi, "wansha");
        shensimayi->tag["JilveWansha"] = true;
    }else
        room->askForUseCard(shensimayi, "@zhiheng", "@jilve-zhiheng");
}

// wansha & zhiheng
class JilveViewAsSkill: public ZeroCardViewAsSkill{
public:
    JilveViewAsSkill():ZeroCardViewAsSkill(""){

    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return player->usedTimes("JilveCard") < 2 && player->getMark("@bear") > 0;
    }

    virtual const Card *viewAs() const{
        return new JilveCard;
    }
};

class Jilve: public TriggerSkill{
public:
    Jilve():TriggerSkill("jilve"){
        events << CardUsed << CardResponsed // jizhi
                << AskForRetrial // guicai
                << Damaged; // fangzhu

        view_as_skill = new JilveViewAsSkill;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return TriggerSkill::triggerable(target)
                && target->getMark("@bear") > 0;
    }

    virtual bool trigger(TriggerEvent event, Room* room, ServerPlayer *player, QVariant &data) const{
        player->setMark("JilveEvent",(int)event);
        if(event == CardUsed || event == CardResponsed){
            CardStar card = NULL;
            if(event == CardUsed)
                card = data.value<CardUseStruct>().card;
            else
                card = data.value<CardStar>();

            if(card->isNDTrick() && player->askForSkillInvoke("jilve", data)){
                player->loseMark("@bear");
                room->playSkillEffect(objectName());
                player->drawCards(1);
            }
        }else if(event == AskForRetrial){
            const TriggerSkill *guicai = Sanguosha->getTriggerSkill("guicai");
            if(guicai && !player->isKongcheng() && player->askForSkillInvoke("jilve", data)){
                player->loseMark("@bear");
                guicai->trigger(event, room, player, data);
            }
        }else if(event == Damaged){
            const TriggerSkill *fangzhu = Sanguosha->getTriggerSkill("fangzhu");
            if(fangzhu && player->askForSkillInvoke("jilve", data)){
                player->loseMark("@bear");
                fangzhu->trigger(event, room, player, data);
            }
        }
        player->setMark("JilveEvent",0);
        return false;
    }

    virtual Location getLocation() const{
        return Right;
    }
};

class JilveClear: public PhaseChangeSkill{
public:
    JilveClear():PhaseChangeSkill("#jilve-clear"){

    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return PhaseChangeSkill::triggerable(target)
                && target->getPhase() == Player::NotActive
                && target->tag.value("JilveWansha").toBool();
    }

    virtual bool onPhaseChange(ServerPlayer *target) const{
        target->getRoom()->detachSkillFromPlayer(target, "wansha");
        target->tag.remove("JilveWansha");
        return false;
    }
};

class Lianpo: public PhaseChangeSkill{
public:
    Lianpo():PhaseChangeSkill("lianpo"){
    }

    virtual int getPriority() const{
        return -1;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return target && target->getPhase() == Player::NotActive;
    }

    virtual bool onPhaseChange(ServerPlayer *player) const{
        Room *room = player->getRoom();
        ServerPlayer *shensimayi = room->findPlayerBySkillName("lianpo");
        if(shensimayi == NULL || shensimayi->getMark("lianpo") <= 0)
            return false;

        int n = shensimayi->getMark("lianpo");
        shensimayi->setMark("lianpo", 0);

        if(!shensimayi->askForSkillInvoke("lianpo"))
            return false;

        LogMessage log;
        log.type = "#LianpoCanInvoke";
        log.from = shensimayi;
        log.arg = QString::number(n);
        log.arg2 = objectName();
        room->sendLog(log);
        room->playSkillEffect(objectName());

        PlayerStar p = shensimayi;
        room->setTag("LianpoInvoke", QVariant::fromValue(p));

        return false;
    }
};

class LianpoGet: public PhaseChangeSkill{
public:
    LianpoGet():PhaseChangeSkill("#lianpo-get"){
    }

    virtual int getPriority() const{
        return -3;
    }

    virtual bool triggerable(const ServerPlayer *target) const{
        return target && target->getPhase() == Player::NotActive;
    }

    virtual bool onPhaseChange(ServerPlayer *player) const{
        Room *room = player->getRoom();
        if(!room->getTag("LianpoInvoke").isNull())
        {
            PlayerStar target = room->getTag("LianpoInvoke").value<PlayerStar>();
            room->removeTag("LianpoInvoke");
            if(target->isAlive())
                target->gainAnExtraTurn();
        }
        return false;
    }
};

class Juejing: public DrawCardsSkill{
public:
    Juejing():DrawCardsSkill("juejing"){
        frequency = Compulsory;
    }

    virtual int getDrawNum(ServerPlayer *player, int n) const{
        if(player->isWounded())
            player->getRoom()->playSkillEffect(objectName());
        return n + player->getLostHp();
    }
};

class JuejingKeep: public MaxCardsSkill{
public:
    JuejingKeep():MaxCardsSkill("#juejing"){

    }

    virtual int getExtra(const Player *target) const{
        if(target->hasSkill(objectName()))
            return 2;
        else
            return 0;
    }
};

class Longhun: public ViewAsSkill{
public:
    Longhun():ViewAsSkill("longhun"){

    }

    virtual bool isEnabledAtResponse(const Player *player, const QString &pattern) const{
        return pattern == "slash"
                || pattern == "jink"
                || pattern.contains("peach")
                || pattern == "nullification";
    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return player->isWounded() || Slash::IsAvailable(player);
    }

    virtual bool viewFilter(const QList<CardItem *> &selected, const CardItem *to_select) const{
        const Card *card = to_select->getFilteredCard();
        int n = qMax(1, Self->getHp());

        if(selected.length() >= n)
            return false;

        if(n > 1 && !selected.isEmpty()){
            Card::Suit suit = selected.first()->getFilteredCard()->getSuit();
            return card->getSuit() == suit;
        }

        switch(ClientInstance->getStatus()){
        case Client::Playing:{
                if(Self->isWounded() && card->getSuit() == Card::Heart)
                    return true;
                else if(Slash::IsAvailable(Self) && card->getSuit() == Card::Diamond){
                    if(card == Self->getWeapon() && card->objectName() == "crossbow")
                        return Self->canSlashWithoutCrossbow();
                    else
                        return !card->hasFlag("isUsing");
                }
                else
                    return false;
            }

        case Client::Responsing:{
                QString pattern = ClientInstance->getPattern();
                if(pattern == "jink")
                    return card->getSuit() == Card::Club;
                else if(pattern == "nullification")
                    return card->getSuit() == Card::Spade;
                else if(pattern == "peach" || pattern == "peach+analeptic")
                    return card->getSuit() == Card::Heart;
                else if(pattern == "slash")
                    return card->getSuit() == Card::Diamond;
            }

        default:
            break;
        }

        return false;
    }

    virtual const Card *viewAs(const QList<CardItem *> &cards) const{
        int n = qMax(1, Self->getHp());

        if(cards.length() != n)
            return NULL;

        const Card *card = cards.first()->getFilteredCard();
        Card *new_card = NULL;

        Card::Suit suit = card->getSuit();
        int number = cards.length() > 1 ? 0 : card->getNumber();
        switch(card->getSuit()){
        case Card::Spade:{
                new_card = new Nullification(suit, number);
                break;
            }

        case Card::Heart:{
                new_card = new Peach(suit, number);
                break;
            }

        case Card::Club:{
                new_card = new Jink(suit, number);
                break;
            }

        case Card::Diamond:{
                new_card = new FireSlash(suit, number);
                break;
            }
        default:
            break;
        }

        if(new_card){
            new_card->setSkillName(objectName());
            new_card->addSubcards(cards);
        }

        return new_card;
    }

    virtual int getEffectIndex(const ServerPlayer *, const Card *card) const{
        return static_cast<int>(card->getSuit()) + 1;
    }
};

class JuejingEx: public TriggerSkill{
public:
    JuejingEx():TriggerSkill("juejingEx"){
        events << CardLostOneTime << CardGotOneTime
               << CardDiscarded
               << PhaseChange
               << CardDrawnDone;
        frequency = Compulsory;
    }

    void dojuejingEx(ServerPlayer *shenzhao, Room *room) const{
        int HandcardNum = shenzhao->getHandcardNum();
        if(HandcardNum < 4){
            LogMessage log;
            log.type = "#TriggerSkill";
            log.from = shenzhao;
            log.arg = "juejingEx";
            room->sendLog(log);

            shenzhao->drawCards(4 - HandcardNum);
        }else if(HandcardNum > 4){
            LogMessage log;
            log.type = "#TriggerSkill";
            log.from = shenzhao;
            log.arg = "juejingEx";
            room->sendLog(log);

            room->askForDiscard(shenzhao,"juejingEx",HandcardNum - 4, HandcardNum - 4);
        }
    }


    virtual bool trigger(TriggerEvent event, Room* room, ServerPlayer *player, QVariant &data) const{

        if(event == PhaseChange && player->getPhase()==Player::Draw){
            return true;

        }else if(event == CardLostOneTime && player->getPhase() != Player::Discard){
            CardsMoveOneTimeStar move = data.value<CardsMoveOneTimeStar>();
            if(move->from_places.contains(Player::Hand))
                dojuejingEx(player, room);

        }else if(event == CardGotOneTime){
            CardsMoveOneTimeStar move = data.value<CardsMoveOneTimeStar>();
            if(move->to_place == Player::Hand)
                dojuejingEx(player, room);

        }else if(player->getPhase() != Player::Discard){
            dojuejingEx(player, room);
        }
        return false;
    }
};

class LonghunEx: public ViewAsSkill{
public:
    LonghunEx():ViewAsSkill("longhunEx"){

    }

    virtual bool isEnabledAtResponse(const Player *player, const QString &pattern) const{
        return pattern == "slash"
                || pattern == "jink"
                || pattern.contains("peach")
                || pattern == "nullification";
    }

    virtual bool isEnabledAtPlay(const Player *player) const{
        return player->isWounded() || Slash::IsAvailable(player);
    }

    virtual bool viewFilter(const QList<CardItem *> &selected, const CardItem *to_select) const{
        const Card *card = to_select->getFilteredCard();
        int n = 1;

        if(selected.length() >= n)
            return false;

        switch(ClientInstance->getStatus()){
        case Client::Playing:{
                if(Self->isWounded() && card->getSuit() == Card::Heart)
                    return true;
                else if(Slash::IsAvailable(Self) && card->getSuit() == Card::Diamond){
                    if(card == Self->getWeapon() && card->objectName() == "crossbow")
                        return Self->canSlashWithoutCrossbow();
                    else
                        return !card->hasFlag("isUsing");
                }
                else
                    return false;
            }

        case Client::Responsing:{
                QString pattern = ClientInstance->getPattern();
                if(pattern == "jink")
                    return card->getSuit() == Card::Club;
                else if(pattern == "nullification")
                    return card->getSuit() == Card::Spade;
                else if(pattern == "peach" || pattern == "peach+analeptic")
                    return card->getSuit() == Card::Heart;
                else if(pattern == "slash")
                    return card->getSuit() == Card::Diamond;
            }

        default:
            break;
        }

        return false;
    }

    virtual const Card *viewAs(const QList<CardItem *> &cards) const{
        int n = 1;

        if(cards.length() != n)
            return NULL;

        const Card *card = cards.first()->getFilteredCard();
        Card *new_card = NULL;

        Card::Suit suit = card->getSuit();
        int number = cards.length() > 1 ? 0 : card->getNumber();
        switch(card->getSuit()){
        case Card::Spade:{
                new_card = new Nullification(suit, number);
                break;
            }

        case Card::Heart:{
                new_card = new Peach(suit, number);
                break;
            }

        case Card::Club:{
                new_card = new Jink(suit, number);
                break;
            }

        case Card::Diamond:{
                new_card = new FireSlash(suit, number);
                break;
            }
        default:
            break;
        }

        if(new_card){
            new_card->setSkillName(objectName());
            new_card->addSubcards(cards);
        }

        return new_card;
    }

    virtual int getEffectIndex(const ServerPlayer *, const Card *card) const{
        return static_cast<int>(card->getSuit()) + 1;
    }
};

class DuojianQG: public TriggerSkill{
public:
   DuojianQG():TriggerSkill("#duojianQG"){
        events << TurnStart;
    }

   virtual bool trigger(TriggerEvent, Room* room, ServerPlayer *player, QVariant &) const{
       if(!player->faceUp())
           return false;

       QList<ServerPlayer *> otherplayers = room->getOtherPlayers(player);
       QList<ServerPlayer *> allplayers = room->getAllPlayers();
       foreach(ServerPlayer *people,otherplayers){
           foreach(const Card *equip, people->getEquips()){
               if(equip->inherits("QinggangSword")){
                   if(room->askForSkillInvoke(player,"duojianQG")){
                       room->playSkillEffect("longhunEx", 5);
                       player->obtainCard(equip);
                   }
                   return false;
               }
           }
       }
       foreach(ServerPlayer *people,allplayers){
           foreach(const Card *equip, people->getJudgingArea()){
               if(equip->inherits("QinggangSword")){
                   if(room->askForSkillInvoke(player,"duojianQG")){
                       room->playSkillEffect("longhunEx", 5);
                       player->obtainCard(equip);
                   }
                   return false;
               }
           }
       }
       return false;
   }
};

GodPackage::GodPackage()
    :Package("god")
{
    General *shenguanyu = new General(this, "shenguanyu", "god", 5);
    shenguanyu->addSkill(new Wushen);
    shenguanyu->addSkill(new Wuhun);
    shenguanyu->addSkill(new WuhunRevenge);

    related_skills.insertMulti("wuhun", "#wuhun");

    General *shenlvmeng = new General(this, "shenlvmeng", "god", 3);
    shenlvmeng->addSkill(new Shelie);
    shenlvmeng->addSkill(new Gongxin);

    General *shenzhouyu = new General(this, "shenzhouyu", "god");
    shenzhouyu->addSkill(new Qinyin);
    shenzhouyu->addSkill(new MarkAssignSkill("@flame", 1));
    shenzhouyu->addSkill(new GreatYeyan);
    shenzhouyu->addSkill(new MediumYeyan);
    shenzhouyu->addSkill(new SmallYeyan);

    General *shenzhugeliang = new General(this, "shenzhugeliang", "god", 3);
    shenzhugeliang->addSkill(new Qixing);
    shenzhugeliang->addSkill(new QixingStart);
    shenzhugeliang->addSkill(new QixingAsk);
    shenzhugeliang->addSkill(new QixingClear);
    shenzhugeliang->addSkill(new Kuangfeng);
    shenzhugeliang->addSkill(new Dawu);

    related_skills.insertMulti("qixing", "#qixing");
    related_skills.insertMulti("qixing", "#qixing-ask");
    related_skills.insertMulti("qixing", "#qixing-clear");

    General *shencaocao = new General(this, "shencaocao", "god", 3);
    shencaocao->addSkill(new Guixin);
    shencaocao->addSkill(new Feiying);

    General *shenlvbu = new General(this, "shenlvbu", "god", 5);
    shenlvbu->addSkill(new Kuangbao);
    shenlvbu->addSkill(new MarkAssignSkill("@wrath", 2));
    shenlvbu->addSkill(new Wumou);
    shenlvbu->addSkill(new Wuqian);
    shenlvbu->addSkill(new Shenfen);

    related_skills.insertMulti("kuangbao", "#@wrath-2");

    General *shenzhaoyun = new General(this, "shenzhaoyun", "god", 2);
    shenzhaoyun->addSkill(new Juejing);
    shenzhaoyun->addSkill(new JuejingKeep);
    shenzhaoyun->addSkill(new Longhun);
    shenzhaoyun->addSkill(new TransfigureSkill("longpo", "shenzhaoyun", "chudaishenzhaoyun"));
    related_skills.insertMulti("juejing", "#juejing");

    General *shensimayi = new General(this, "shensimayi", "god", 4);
    shensimayi->addSkill(new Renjie);
    shensimayi->addSkill(new Baiyin);
    shensimayi->addSkill(new Lianpo);
    shensimayi->addSkill(new LianpoCount);
    shensimayi->addSkill(new LianpoGet);

    shensimayi->addRelateSkill("jilve");

    related_skills.insertMulti("jilve", "#jilve-clear");
    related_skills.insertMulti("lianpo", "#lianpo-count");
    related_skills.insertMulti("lianpo", "#lianpo-get");

    addMetaObject<GongxinCard>();
    addMetaObject<GreatYeyanCard>();
    addMetaObject<ShenfenCard>();
    addMetaObject<GreatYeyanCard>();
    addMetaObject<MediumYeyanCard>();
    addMetaObject<SmallYeyanCard>();
    addMetaObject<WushenSlash>();
    addMetaObject<KuangfengCard>();
    addMetaObject<DawuCard>();
    addMetaObject<WuqianCard>();
    addMetaObject<JilveCard>();

    skills << new Jilve << new JilveClear;
}

NGodPackage::NGodPackage()
    :Package("ngod")
{
    General *chudaishenzhaoyun = new General(this, "chudaishenzhaoyun", "god", 1, true, true);
    chudaishenzhaoyun->addSkill(new JuejingEx);
    chudaishenzhaoyun->addSkill(new LonghunEx);
    chudaishenzhaoyun->addSkill(new DuojianQG);

    related_skills.insertMulti("longhunEx", "#duojianQG");
}

ADD_PACKAGE(God)
ADD_PACKAGE(NGod)
