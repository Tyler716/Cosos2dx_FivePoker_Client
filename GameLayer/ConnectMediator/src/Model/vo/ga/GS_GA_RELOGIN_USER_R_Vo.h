#ifndef _GS_GA_RELOGIN_USER_R_VO_H_
#define _GS_GA_RELOGIN_USER_R_VO_H_

#include "Type.h"

struct GS_GA_RELOGIN_USER_R_Vo
{
    // 回傳狀態(0:失敗, 1:成功)
    Int state;

    enum
    {
        STATE_FAIL = 0,
        STATE_OK = 1
    };
};

#endif //_GS_GA_RELOGIN_USER_R_VO_H_
