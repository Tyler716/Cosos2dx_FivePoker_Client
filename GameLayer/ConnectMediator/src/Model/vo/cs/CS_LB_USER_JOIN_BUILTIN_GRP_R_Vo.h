#ifndef _CS_LB_USER_JOIN_BUILTIN_GRP_R_VO_H_
#define _CS_LB_USER_JOIN_BUILTIN_GRP_R_VO_H_

#include "type.h"
#include "../../../Common/GameProtocol.h"


struct CS_LB_USER_JOIN_BUILTIN_GRP_R_Vo
{
    GAME_TYPE           GameType;               //< 遊戲類型
    GAME_GRP_MODE       GameMode;               //< 遊戲模式
    Int                 State;                  //< 0:失敗, 1:成功, 2:遊戲暫停開放, 3:玩家點數不足, 4:遊戲維護中, 5:主機即將維護, 6:此帳號屬性未開放本遊戲進行, 7:局數索引值超過範圍, 8: 已達可入局的數目上限, 9: 玩家沒有進入該局的權限, 10:局數索引值超過範圍, 11: 該桌位已有玩家加入
    Int                 Money;                  //< 需要的點數(當State = 3)
    Int                 JoinGrpIndex;           //< 加入的局的索引號
    UInt                GameWindowID;           //< 遊戲視窗識別碼(支援遊戲視窗多開，記錄本次請求為哪個遊戲視窗提出)
};

#endif //_CS_LB_USER_JOIN_BUILTIN_GRP_R_VO_H_
