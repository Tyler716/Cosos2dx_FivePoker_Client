#ifndef _CS_LB_ASK_GAME_GRP_LIST_S_VO_H_
#define _CS_LB_ASK_GAME_GRP_LIST_S_VO_H_

#include "type.h"

struct CS_LB_ASK_GAME_GRP_LIST_S_Vo
{
    GAME_TYPE           GameType;               //< 遊戲類型
    GAME_GRP_MODE       GameMode;               //< 遊戲模式
    Int                 GrpFmNo;                //< 從第幾局的索引值(GrpFmNo到GrpEdNo最多六桌)
    Int                 GrpEdNo;                //< 到第幾局的索引值
};

#endif //_CS_LB_ASK_GAME_GRP_LIST_S_VO_H_
