#ifndef _GCLIMSG_VO_H_
#define _GCLIMSG_VO_H_

#include "Type.h"
#include "SLMSG_Vo.h"
#include <wchar.h>

// 手牌儲存結構。
typedef unsigned int POKERHAND[5];

//下注失敗
struct GCliMsgBetFail_Vo
{
    Int         ErrorCode;              //0:額度不足
};

//是否可以保留機台
struct GCliCanKeepTable_Vo
{
    Int         State;                  // 0: 無法保留機台, 1: 可以保留機台
};

//第一次開牌資料
struct GCliMsgDeal_Vo
{
    POKERHAND   PokerHand;
    Int         Hold[5];                //建議保留的牌張
    UInt        PokerType;              //牌型
	Int         UserPoint;              //下注後的分數
};

//第二次開牌資料
struct GCliMsgDraw_Vo
{
    POKERHAND   PokerHand;
    UInt        PokerType;              //中獎牌型
    Int64         WinLosePoint;           //得分點數
    Int         CanDoubleGame;          //可以玩比倍遊戲嗎? 0:不行 1:可以
    Int         CanMiniGame;//可以玩小遊戲嗎? 0:不行 1:可以玩第一種 2:可以玩第二種
};

//比倍遊戲資料
struct GCliMsgDoubleGameData_Vo
{
    UInt        PokerHand;              //比倍用的電腦牌
};

//比倍遊戲結果
struct GCliMsgDoubleGameResult_Vo
{
    POKERHAND   PokerHand;              //比倍用的5張牌 0:電腦牌 1:玩家選的牌的值 2~4:本來蓋住的牌的值
    Int         WinLosePoint;           //比倍過後的得分點數
};

//GCLIMSG_SET_MULTIPLE_RESULT   設定倍率的結果
struct CGGCliSetMultipleResult_Vo
{
    Int Succ;//成功或失敗
    Int MultipleIndex;//倍數索引 1~4 (成功傳改變後的倍數, 失敗則傳當前的倍數)
    Int GameScore_1[11];                //遊戲得分表 - 1分
    Int GameScore_2[11];                //遊戲得分表 - 2分
    Int GameScore_3[11];                //遊戲得分表 - 3分
    Int GameScore_4[11];                //遊戲得分表 - 4分
    Int GameScore_5[11];                //遊戲得分表 - 5分
};

//GCLIMSG_MINI_GAME_RESULT      = 6016,//小遊戲結果
struct GCliMiniGameResult_Vo
{
    Int64 PrevWinPoint;//玩遊戲前的勝利點
    Int64 WinPoint;//遊戲過後的勝利點
    Int Bouns;//獎項類型
};

//Wait To Sync Game, 等待到遊戲畫面
struct GCliWSGame_Vo : GCliWSGameBase_Vo
{
    Int         TableNo;                //桌號
    Int         GameScore_1[11];        //遊戲得分表 - 1分
    Int         GameScore_2[11];        //遊戲得分表 - 2分
    Int         GameScore_3[11];        //遊戲得分表 - 3分
    Int         GameScore_4[11];        //遊戲得分表 - 4分
    Int         GameScore_5[11];        //遊戲得分表 - 5分
    Int         GameRatio[4];           //遊戲基本注額(1倍所指的額度)
    Int         GameRatioIndex;         //當前注額索引
    Int         MinBet;                 //最小押分
    Int         MaxBet;                 //最大押分
    Int         BetRatio;               //倍率
    Int         PointRatioL;            //左比率
    Int         PointRatioR;            //右比率
    wchar_t     group_unique_id_[32];   //將號
};

//GameBalance 等待至遊戲結算畫面
struct GCliWSGameBalance_Vo : GCliWSGameBalanceBase_Vo
{
    Int         nUsersWinLosePoints;    //輸贏的點數
    Int         nUsersAmountPoints;     //總計
};

//GroupBalance 等待至局結算幕
struct GCliWSGroupBalance_Vo : GCliWSGroupBalanceBase
{
    UInt        uUsersEntryPoints;      //原有點數
    Int         nUsersAmountPoints;     //各家總計
};

#endif //_GCLIMSG_VO_H_
