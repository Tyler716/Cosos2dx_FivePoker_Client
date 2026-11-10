#ifndef _GSLMSG_VO_H_
#define _GSLMSG_VO_H_

#include "Type.h"
#include <wchar.h>

//GSLMSG_BET 玩家下注->開牌
struct GSLBet_Vo
{
    Int     Bet;            //下注分數 1~5
};

//GSLMSG_DRAW 玩家保留->開牌
struct GSLDraw_Vo
{
    Int     Hold[5];        //保留的牌張
};

//GSLMSG_DOUBLE_GAME 比倍
struct GSLDoubleGame_Vo
{
    Int     SelectIndex;    //玩家選擇的牌 1~4
};

//GSLMSG_SET_MULTIPLE   設定倍率
struct GSLSetMultiple_Vo
{
    Int     MultipleIndex;  //倍數索引 1~4
};

//GSLMSG_EXIT_GAME
struct GSLExitGame_Vo
{
    Int     iHoldGame;      // 0:不保留機台  1:保留機台
};

// GSLMSG_TALK
struct GSLTalk_Vo
{
    wchar_t TalkMsg[64];    // 聊天訊息
};

#endif //_GSLMSG_VO_H_
