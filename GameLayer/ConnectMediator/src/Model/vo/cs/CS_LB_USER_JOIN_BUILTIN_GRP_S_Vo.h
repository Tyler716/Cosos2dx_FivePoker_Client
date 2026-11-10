#ifndef _CS_LB_USER_JOIN_BUILTIN_GRP_S_VO_H_
#define _CS_LB_USER_JOIN_BUILTIN_GRP_S_VO_H_

#include "Type.h"
#include "GameProtocol.h"

struct CS_LB_USER_JOIN_BUILTIN_GRP_S_Vo
{
    GAME_TYPE           GameType;               //< 遊戲類型
    GAME_GRP_MODE       GameMode;               //< 遊戲模式
    Int                 CSGrpNo;                //< 局號
    Int                 SeatNo;                 //< 座位號
    COIN_TYPE           CoinType;               //< 0:pk幣, 1:免費幣
    Int                 KeepTable;              //< 是否為斷線重連. 0: 否, 1: 是
    UInt                GameWindowID;           //< 遊戲視窗識別碼(支援遊戲視窗多開，記錄本次請求為哪個遊戲視窗提出)
};

#endif //_CS_LB_USER_JOIN_BUILTIN_GRP_S_VO_H_
