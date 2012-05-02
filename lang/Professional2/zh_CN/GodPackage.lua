-- translation for GodPackage

return {
	["god"] = "神",

	["#shenguanyu"] = "鬼神再临",
	["shenguanyu"] = "神关羽",
	["wushen"] = "武神",
	[":wushen"] = "<b>锁定技</b>，你的每一张红桃手牌都视为【杀】，你使用红桃【杀】时无距离限制",
	["wuhun"] = "武魂",
	[":wuhun"] = "<b>锁定技</b>，任意角色每对你造成1点伤害，获得一个梦魇标记，你死亡时，令持有最多该标记的一名角色判定，除非结果为【桃】或【桃园结义】，否则该角色立即死亡。",
	["@nightmare"] = "梦魇",
	["#WuhunRevenge"] = "%from 的锁定技【%arg2】被触发，持有最多梦魇标记的角色 %to(%arg个)将立即死亡",

	["#shenlvmeng"] = "圣光之国士",
	["shenlvmeng"] = "神吕蒙",
	["shelie"] = "涉猎",
	[":shelie"] = "摸牌阶段开始时，你可以选择执行以下行动来取代摸牌：从牌堆顶亮出五张牌，拿走不同花色的牌各一张，然后将其余的置入弃牌堆",
	["gongxin"] = "攻心",
	[":gongxin"] = "出牌阶段，你可以观看一次任意一名角色的手牌，并可以展示其中一张红桃牌，然后弃置它或将它置于牌堆顶",
	["shelie:yes"] = "选择执行以下行动来取代摸牌：从牌堆顶亮出五张牌，拿走不同花色的牌各一张，然后将其余的置入弃牌堆",
	[":gongxin:"] = "请问如何处理这张红桃牌:",
	["gongxin:discard"] = "弃置",
	["gongxin:put"] = "放置在牌堆顶",

	["#shenzhouyu"] = "赤壁的火神",
	["shenzhouyu"] = "神周瑜",
	["qinyin"] = "琴音",
	[":qinyin"] = "弃牌阶段结束时，当你弃置了两张或更多的手牌，可令所有角色各回复1点体力或各失去1点体力\
★失去体力意味着相关的武将技能不能发动。例如遗计，反馈等等……失去体力不等于受到伤害\
★发动琴音时只能选择其中的1项执行，所有人回复1点体力或所有人失去一点体力（包括周瑜自己）",
	["qinyin:yes"] = "所有角色各回复1点体力或各失去1点体力",
	[":qinyin:"] = "请选择琴音技能的选项:",
	["qinyin:up"] = "令所有角色各回复1点体力",
	["qinyin:down"] = "所有角色各失去1点体力",
	["yeyan"] = "业炎",
	["@flame"] = "业炎",
	["greatyeyan"] = "大业炎",
	[":greatyeyan"] = "出牌阶段，你可选择1名角色，弃置4张不同花色的手牌并失去3点体力，对其造成3点火焰伤害。整局游戏只能发动一次，中业炎和小业炎也不能再发动",
	["mediumyeyan"] = "中业炎",
	[":mediumyeyan"] = "出牌阶段，你可以选择1~2名角色，先弃置4张不同花色的手牌并失去3点体力，然后对第一名角色造成2点火焰伤害，对第二名角色（若有）造成1点火焰伤害，整局游戏只能使用一次，发动后，大业炎和小业炎也将不能发动",
	["smallyeyan"] = "小业炎",
	[":smallyeyan"] = "出牌阶段，你可以选择1~3名角色，对他们每人造成1点火焰伤害，整局游戏只能发动一次，发动后，大业炎和中业炎将不能再发动",

	["#shenzhugeliang"] = "赤壁的妖术师",
	["shenzhugeliang"] = "神诸葛亮",
	["qixing"] = "七星",
	[":qixing"] = "游戏开始前，共发你11张牌，选4张作为手牌，其余的面朝下置于一旁（移出游戏），称之为“星”。每当你于摸牌阶段摸牌后，可用任意数量的手牌等量交换这些“星”\
★“星”不属于游戏中的牌，所以“星”不能被锦囊和武将技能影响",
	["@star"] = "七星",
	["stars"] = "七星堆",
	["kuangfeng"] = "狂风",
	[":kuangfeng"] = "回合结束阶段开始时，你可以弃掉1枚“星”指定一名角色：直到你的下回合开始，该角色每次受到的火焰伤害+1",
	["@gale"] = "狂风",
	["dawu"] = "大雾",
	[":dawu"] = "回合结束阶段开始时，你可以弃掉X枚“星”指定X名角色：直到你的下回合开始，防止他们受到的除雷电伤害外的一切伤害\
★“星”不属于游戏中的牌，所以“星”不能被锦囊和武将技能影响\
★目前的雷电伤害有“雷杀”，张角的技能“雷击”，以及“闪电”三种\
★大雾不能防止失去体力",
	["@fog"] = "大雾",
	["@qixing-exchange"] = "请选择 %arg 张用以交换的手牌",
	["@kuangfeng-card"] = "请选择一名角色，直到你的下回合开始，该角色每次受到的火焰伤害+1",
	["@dawu-card"] = "你可以弃掉X枚“星”指定X名角色：直到你的下回合开始，防止他们受到的除雷电伤害外的一切伤害",
	["#QixingExchange"] = "%from 使用了【%arg2】，交换了 %arg 张手牌",
	["#FogProtect"] = "%from 受到大雾保护，防止了 %arg 点伤害[%arg2]",
	["#GalePower"] = "由于狂风的影响，%from 的伤害从 %arg 点火焰伤害增加到 %arg2 点火焰伤害",

	["#shencaocao"] = "超世之英杰",
	["shencaocao"] = "神曹操",
	["guixin"] = "归心",
	[":guixin"] = "每当你受到1点伤害，可分别从每名其他角色区域处获得一张牌，然后将你的武将牌翻面",
	["feiying"] = "飞影",
	[":feiying"] = "<b>锁定技</b>，当其他角色计算与你的距离时，始终+1",
	["guixin:yes"] = "你可分别从每名其他角色区域处取得一张牌，然后将你的武将牌翻面",

	["#shenlvbu"] = "修罗之道",
	["shenlvbu"] = "神吕布",
	["kuangbao"] = "狂暴",
	[":kuangbao"] = "游戏开始时你获得2个暴怒标记，游戏中你每受到或造成1点伤害，获得1个暴怒标记",
	["@wrath"] = "暴怒",
	["wumou"] = "无谋",
	[":wumou"] = "<b>锁定技</b>，你每使用一张非延时类锦囊（在它结算前），弃掉1个暴怒标记或失去1点体力。",
	["wuqian"] = "无前",
	[":wuqian"] = "出牌阶段你可以弃掉两个暴怒标记并指定一名角色，这名角色的防具无效且你获得“无双”的技能，直到回合结束",
	["shenfen"] = "神愤",
	[":shenfen"] = "出牌阶段，弃掉六个暴怒标记，你对每名其他角色各造成一点伤害，其他角色先弃置各自装备区里所有的牌，再各弃置四张手牌，然后将你的武将牌反面，每阶段限一次",
	["#KuangbaoDamage"] = "%from 的锁定技【%arg2】被触发，造成 %arg 点伤害获得 %arg 枚暴怒标记",
	["#KuangbaoDamaged"] = "%from 的锁定技【%arg2】被触发，受到 %arg 点伤害获得 %arg 枚暴怒标记",
	["wumou:discard"] = "弃掉一个“暴怒”标记",
	["wumou:losehp"] = "失去一点体力",

	["#shenzhaoyun"] = "神威如龙",
	["shenzhaoyun"] = "神赵云",
	["juejing"] = "绝境",
	[":juejing"] = "<b>锁定技</b>，摸牌阶段，你摸牌的数量为你已损失的体力值+2；你的手牌上限始终+2",
	["longhun"] = "龙魂",
	[":longhun"] = "你可以将同花色的X张牌按下列规则使用（或打出）：红桃当【桃】，方块当火属性的【杀】，梅花当【闪】，黑桃当【无懈可击】（X为你的当前体力值且至少为1）",

	["#shensimayi"] = "晋国之祖",
	["shensimayi"] = "神司马懿",
	["renjie"] = "忍戒",
	[":renjie"] = "<b>锁定技</b>，当你受到伤害或于弃牌阶段弃牌时，获得等同于受到伤害（或弃牌）等量的“忍”标记",
	["@bear"] = "忍",
	["baiyin"] = "拜印",
	[":baiyin"] ="<b>觉醒技</b>，回合开始阶段开始时，若你拥有4个或更多的忍标记，须减1点体力上限，并获得技能“极略”（弃掉一枚忍标记发动下列一项技能：“鬼才”、“放逐”、“完杀”、“制衡”、“集智”）",
	["#BaiyinWake"] = "%from 的忍标记达到 %arg 个(大于或等于4)，达到拜印的觉醒条件",
	["jilve"] = "极略",
	[":jilve"] = "弃掉一枚忍标记发动下列一项技能：“鬼才”、“放逐”、“完杀”、“制衡”、“集智”",
	["lianpo"] = "连破",
	[":lianpo"] = "若你于一回合内杀死至少一名角色，可于此回合结束后，进行一个额外的回合",
	["@jilve-zhiheng"] = "请发动【制衡】技能",
	["#LianpoCanInvoke"] = "%from 在本回合内杀死了 %arg 名角色，达成【%arg2】的发动条件",
	["#LianpoRecord"] = "%from 在 %arg 的回合内杀死了 %to，将可在 %arg 回合结束后进行一个额外的回合",

-- CV
	["cv:shenguanyu"] = "官方",
	["$wushen1"] = "还不速速领死。",
	["$wushen2"] = "取汝狗头犹如探囊取物。",
	["$wuhun1"] = "拿命来。",
	["$wuhun2"] = "谁来与我同去。",
	["~shenguanyu"] = "吾一世英名，竟葬于小人之手！",

	["cv:shenlvmeng"] = "官方",
	["$shelie1"] = "略懂略懂。",
	["$shelie2"] = "什么都略懂一点，生活更多彩一些。",
	["$gongxin1"] = "攻城为下，攻心为上。",
	["$gongxin2"] = "我替施主把把脉。",
	["~shenlvmeng"] = "劫数难逃，我们别无选择……",

	["cv:shenzhouyu"] = "官方",
	["$qinyin1"] = "琴声",
	["$qinyin2"] = "琴声",
	["$greatyeyan"] = "让这熊熊业火，焚尽你的罪恶！",
	["$mediumyeyan"] = "聆听吧，这首献给你的镇魂曲！",
	["$smallyeyan"] = "琴声",
	["~shenzhouyu"] = "逝者不死，浴火重生……",

	["cv:shenzhugeliang"] = "官方",
	["$qixing1"] = "伏望天恩，誓讨汉贼！",
	["$qixing2"] = "起星辰之力，佑我蜀汉！",
	["$dawu1"] = "此计可保你一时平安",
	["$dawu2"] = "此非万全之策，惟惧天雷",
	["$kuangfeng1"] = "万事俱备，只欠业火",
	["$kuangfeng2"] = "风~~~~~起~~~~~~！",
	["~shenzhugeliang"] = "今当远离……临表涕零……不知所言……",

	["cv:shencaocao"] = "官方",
	["$guixin"] = "山不厌高，海不厌深\
周公吐哺，天下归心",
	["$guixin1"] = "周公吐哺，天下归心",
	["$guixin2"] = "山不厌高，海不厌深",
	["~shencaocao"] = "腾蛇乘雾……终为土灰……",

	["cv:shenlvbu"] = "官方",
	["$kuangbao1"] = "(嚎叫声)",
	["$kuangbao2"] = "哼！",
	["$wumou1"] = "哪个说我有勇无谋！？",
	["$wumou2"] = "不管这些了！",
	["$wuqian1"] = "看我神威，无坚不摧！",
	["$wuqian2"] = "天皇老子也保不住你！",
	["$shenfen1"] = "凡人们，颤抖吧！这是神之怒火！！",
	["$shenfen2"] = "这，才是活生生的地狱！",
	["~shenlvbu"] = "我在修罗烈狱等着你们！哈哈哈哈……",

	["cv:shenzhaoyun"] = "猎狐",
	["$juejing"] = "龙战于野,其血玄黄",
	["$longhun1"] = "金甲映日,驱邪祛秽", -- spade
	["$longhun2"] = "腾龙行云,首尾不见", -- club
	["$longhun3"] = "潜龙于渊,涉灵愈伤", -- heart
	["$longhun4"] = "千里一怒,红莲灿世", -- diamond
	["~shenzhaoyun"] = "血染鳞甲,龙坠九天",

	["cv:shensimayi"] = "英雄杀",
	["$renjie"] = "我忍。",
	["$baiyin"] = "君子藏器于身，待时而动。",
	["$jilve"] = "哈哈……",
	["$lianpo"] = "何为人？是为忍。",
    ["~shensimayi"] = "忍不住了。",
}
