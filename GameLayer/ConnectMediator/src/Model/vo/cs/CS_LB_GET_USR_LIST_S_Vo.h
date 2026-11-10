#ifndef _CS_LB_GET_USR_LIST_S_VO_H_
#define _CS_LB_GET_USR_LIST_S_VO_H_

#include "Type.h"

struct CS_LB_GET_USR_LIST_S_Vo
{
    Int     QueryType;      ///< 查詢模式(0:分頁 1:全部)。(1)
    Int     UsrFmNo;        ///< 從第幾個玩家的索引值(0)
    Int     UsrEdNo;        ///< 到第幾個玩家的索引值(1000)
};

#endif  //_CS_LB_GET_USR_LIST_S_VO_H_