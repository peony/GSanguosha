-- translation for SP Package

return {
	["sp"] = "SP",

	["#yangxiu"] = "恃才放旷",
	["yangxiu"] = "杨修",
	["illustrator:yangxiu"] = "张可",
	["jilei"] = "鸡肋",
	["danlao"] = "啖酪",
	[":jilei"] = "当你受到伤害时，说出一种牌的类别（基本牌、锦囊牌、装备牌），对你造成伤害的角色不能使用、打出或弃置该类别的手牌直到回合结束\
◆弃牌阶段，若仅剩不可弃置的手牌且超出手牌上限时，该角色需展示其手牌。例如，若指定的是装备牌，此时该角色2点体力，同时手里有两张【闪】和三张装备牌，弃牌阶段，该角色必须把两张【闪】弃置，然后该角色展示这三张装备牌，结束他的回合",
	[":danlao"] = "当一个锦囊指定了不止一个目标，且你也是其中之一时，你可以立即摸一张牌，则该锦囊跳过对你的结算",
	["$jilei1"] = "食之无肉，弃之有味。",
	["$jilei2"] = "曹公之意，我已了然。",
	["$danlao1"] = "我喜欢～",
	["$danlao2"] = "来来，一人一口～",
	["~yangxiu"] = "我故自已死之晚也。",
	["cv:yangxiu"] = "官方",
	["#DanlaoAvoid"] = "%from 发动了技能【%arg2】，跳过了锦囊 %arg 对他的结算",
	["#Jilei"] = "%from 鸡肋了 %to 的 %arg",
	["#JileiClear"] = "%from 的鸡肋效果消失",

	["#gongsunzan"] = "白马将军",
	["gongsunzan"] = "公孙瓒",
	["illustrator:gongsunzan"] = "Vincent",
	["yicong"] = "义从",
	[":yicong"] = "<b>锁定技</b>，只要你的体力值大于2点，你计算与其他角色的距离时，始终-1；只要你的体力值为2点或更低，其他角色计算与你的距离时，始终+1",
	["$yicong1"] = "冲啊！",
	["$yicong2"] = "众将听令，摆好阵势，御敌！",
	["~gongsunzan"] = "我军将败，我已无颜苟活于世",

	["#yuanshu"] = "仲家帝",
	["yuanshu"] = "袁术",
	["illustrator:yuanshu"] = "吴昊",
	["yongsi"] = "庸肆",
	[":yongsi"] = "<b>锁定技</b>，摸牌阶段，你额外摸X张牌，X为场上现存势力数。弃牌阶段，你至少弃置等同于场上现存势力数的牌（不足则全弃）",
	["weidi"] = "伪帝",
	[":weidi"] = "<b>锁定技</b>，你视为拥有当前主公的主公技",
	["$yongsi1"] = "玉玺在手，天下我有！",
	["$yongsi2"] = "大汉天下已半入我手！",
	["$yongsi3"] = "我才是皇帝！",
	["$yongsi4"] = "你们都得听我的号令！",
	["$weidi1"] = "我才是皇帝！",
	["$weidi2"] = "你们都得听我的号令！",
	["~yuanshu"] = "可恶，就差一步了……",
	["cv:yuanshu"] = "官方",
	["#YongsiGood"] = "%from 的锁定技【%arg2】被触发，额外摸了 %arg 张牌",
	["#YongsiBad"] = "%from 的锁定技【%arg2】被触发，必须至少弃置 %arg 张牌",
	["#YongsiWorst"] = "%from 的锁定技【%arg2】被触发，弃置了所有的装备和手牌（共 %arg 张）",

	["#sp_diaochan"] = "绝世的舞姬",
	["sp_diaochan"] = "SP貂蝉",
	["illustrator:sp_diaochan"] = "巴萨小马",
	["SP-Diaochan"] = "SP貂蝉",

	["#sp_sunshangxiang"] = "梦醉良缘",
	["sp_sunshangxiang"] = "SP孙尚香",
	["illustrator:sp_sunshangxiang"] = "木美人",
	["chujia"] = "出嫁",
	[":chujia"] = "<b>限定技</b>，游戏开始时，你可以选择变身为SP孙尚香，势力为蜀",
	["cv:sp_sunshangxiang"] = "官方，背碗卤粉",

	["#sp_guanyu"] = "汉寿亭侯",
	["sp_guanyu"] = "SP关羽",
	["illustrator:sp_guanyu"] = "LiuHeng",
	["danji"] = "单骑",
	[":danji"] = "<b>觉醒技</b>，回合开始阶段，若你的手牌数大于你当前的体力值，且本局主公为曹操时，你须减1点体力上限并永久获得技能“马术”。",
	["#DanjiWake"] = "%from 的手牌数(%arg)多于体力值(%arg2)，且本局主公为曹操，达到【单骑】的觉醒条件",

	["#sp_caiwenji"] = "金璧之才",
	["sp_caiwenji"] = "SP蔡文姬",
	["illustrator:sp_caiwenji"] = "木美人",
	["guixiang"] = "归乡",
	[":guixiang"] = "<b>限定技</b>，游戏开始时，你可以选择变身为SP蔡文姬，势力为魏",
	["cv:sp_caiwenji"] = "呼呼",
	
	["#sp_jiaxu"] = "算无遗策",
	["sp_jiaxu"] = "SP贾诩",
	["illustrator:sp_jiaxu"] = "雪君S",
	["guiwei"] = "归魏",
	[":guiwei"] = "<b>限定技</b>，游戏开始时，你可以选择变身为SP贾诩，势力为魏",
	["cv:sp_jiaxu"] = "落凤一箭",
	
	["#sp_pangde"] = "枱榇之悟",
	["sp_pangde"] = "SP庞德",
	["illustrator:sp_pangde"] = "天空之城",
	["pangde_guiwei"] = "归魏",
	[":pangde_guiwei"] = "<b>限定技</b>，游戏开始时，你可以选择变身为SP庞德，势力为魏",
	["cv:sp_pangde"] = "Glory",

	["#sp_machao"] = "西凉的猛狮",
	["sp_machao"] = "SP马超",
	["illustrator:sp_machao"] = "天空之城",
	["fanqun"] = "返群",
	[":fanqun"] = "<b>限定技</b>，游戏开始时，你可以选择变身为SP马超，势力为群",

--hulao mode
	["Hulaopass"] = "虎牢关模式",

	["#shenlvbu1"] = "最强神话",
	["shenlvbu1"] = "虎牢关吕布",
	["illustrator:shenlvbu1"] = "LiuHeng",
	["#shenlvbu2"] = "暴怒的战神",
	["shenlvbu2"] = "虎牢关吕布",
	["illustrator:shenlvbu2"] = "LiuHeng",
	["xiuluo"] = "修罗",
	[":xiuluo"] = "回合开始阶段，你可以弃置一张手牌来将你判定区里的一张延时类锦囊置入弃牌堆(必须花色相同)。",
	["@xiuluo"] = "请弃置一张花色相同的手牌",
	["shenwei"] = "神威",
	[":shenwei"] = "<b>锁定技</b>，摸牌阶段，你额外摸两张牌，你的手牌上限+2。",
	["shenji"] = "神戟",
	[":shenji"] = "没装备武器时，你使用的杀可指定至多3名角色为目标。",

	["#Reforming"] = "%from 进入重整状态",
	["#ReformingRecover"] = "%from 在重整状态中回复了1点体力",
	["#ReformingDraw"] = "%from 在重整状态中摸了1张牌",
	["#ReformingRevive"] = "%from 从重整状态中复活!",
	["draw_1v3"] = "重整摸牌",
	["weapon_recast"] = "武器重铸",
	["Hulaopass:recover"] = "恢复1点体力",
	["Hulaopass:draw"] = "摸1张牌",
	
	["$xiuluo"] = "神挡杀神，佛挡杀佛！",
	["$shenwei"] = "挡我者死！",
	["$shenji"] = "神挡杀神，佛挡杀佛！",
	["~shenlvbu2"] = "呃，不可能！",
	
--sp_card
	["sp_cards"] = "SP卡牌包",
	["sp_moonspear"] = "SP银月枪",
	[":sp_moonspear"] = "装备牌·武器\
攻击范围：３\
武器特效：你的回合外，每当你使用或打出一张黑色手牌时，你可以令你攻击范围内的一名其他角色打出一张【闪】，否则受到你对其造成的1点伤害",
	["@moon-spear-jink"] = "受到SP银月枪技能的影响，你必须打出一张【闪】",
}
