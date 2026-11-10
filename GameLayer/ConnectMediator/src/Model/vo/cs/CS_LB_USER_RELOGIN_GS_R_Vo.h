#ifndef _CS_LB_USER_RELOGIN_GS_R_VO_H_
#define _CS_LB_USER_RELOGIN_GS_R_VO_H_

#include "Type.h"
#include "GameProtocol.h"

struct CS_LB_USER_RELOGIN_GS_R_Vo
{
    STR16                                   IP;                                                                     ///< GSrv IP
    Int                                     PORT;                                                                   ///< GSrv Port
    Int                                     GSGrpNo;                                                ///< 局號(GSGrp No)
    Dword                                   GSGrpUniID;                                                     ///< 局唯一識別碼(動態，隨LS的啟動而重置為0)
    GAME_TYPE                               GameType;                                                   ///< 遊戲型態
    GAME_GRP_MODE                           GameMode;                                               ///< 遊戲模式
    Int                                     SeatNo;                                                             ///< 座位號
    Dword                                   PLUniID;                                                            ///< 玩家唯一識別碼
    UInt                                    GameWindowID;                                                   ///< 遊戲視窗識別碼(支援遊戲視窗多開，記錄本次請求為哪個遊戲視窗提出)
};

#endif //_CS_LB_USER_RELOGIN_GS_R_VO_H_
