#ifndef _CLIMSG_GROUP_SETTED_Vo_H_
#define _CLIMSG_GROUP_SETTED_Vo_H_

#include "Type.h"

struct CLIMSG_GROUP_SETTED_Vo
{
    UInt uMenInGrp;                 //這局的玩家人數
    UInt uUserIndex;                //該玩家在這局的索引值
    Int s32DynPlayers;                  //動態入局嗎? 0:false, 1:true
    Int s32First;                       //第一次局嗎? 0:false, 1:true
};

#endif //_CLIMSG_GROUP_SETTED_Vo_H_
