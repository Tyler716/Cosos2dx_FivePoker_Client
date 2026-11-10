#ifndef _CS_LB_GET_GAME_STATE_R_VO_H_
#define _CS_LB_GET_GAME_STATE_R_VO_H_

#include "Type.h"

struct CS_LB_GET_GAME_STATE_R_Vo
{
    Int     nGames;                                                         ///< 有效的遊戲數目
    //Int     EnableLst[];                                                    ///< GAME_STATE_OPEN: 遊戲開放, GAME_STATE_CLOSE: 遊戲暫停, GAME_STATE_NEW_OPEN:試玩開放, GAME_STATE_NEW_CLOSE:試玩暫停
};

#endif //_CS_LB_GET_GAME_STATE_R_VO_H_
