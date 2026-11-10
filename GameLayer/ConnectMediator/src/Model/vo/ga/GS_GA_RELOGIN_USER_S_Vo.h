#ifndef _GS_GA_RELOGIN_USER_S_VO_H_
#define _GS_GA_RELOGIN_USER_S_VO_H_

#include "Type.h"
#include "GameProtocol.h"

struct GS_GA_RELOGIN_USER_S_Vo
{
    Int             GSGrpNo;                //< 局號(GS GrpNo)
    Dword           GSGrpUniID;             //< 局唯一識別碼(動態，隨GS的啟動而重置為0)
    CARD_TYPE       CardType;               //< 參考CARD_TYPE
    GAME_TYPE       GameType;               //< 遊戲類型
    GAME_GRP_MODE   GameMode;               //< 遊戲模式
    Int             SeatNo;                 //< 座位號
    Dword           PLUniID;                //< 玩家的唯一識別碼
};

#endif //_GS_GA_RELOGIN_USER_S_VO_H_
