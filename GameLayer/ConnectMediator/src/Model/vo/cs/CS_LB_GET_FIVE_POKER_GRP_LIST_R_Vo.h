#ifndef _CS_LB_GET_FIVE_POKER_GRP_LIST_R_VO_H_
#define _CS_LB_GET_FIVE_POKER_GRP_LIST_R_VO_H_

#include "type.h"
#include "CS_FIVE_POKER_GROUP.h"

struct CS_LB_GET_FIVE_POKER_GRP_LIST_R_Vo
{
    GAME_TYPE                   GameType;                   //< 遊戲類型
    GAME_GRP_MODE               GameMode;                   //< 遊戲模式
    Int                         nMaxGrp;                    //< 目前此遊戲已開啟的最大局數
    Int                         GrpFmNo;                    //< 從第幾局的索引值
    Int                         nGrps;                      //< 局數
    //CS_FIVE_POKERY_GROUP         Group[];                    //< 局資訊
};

#endif //_CS_LB_GET_FIVE_POKER_GRP_LIST_R_VO_H_
