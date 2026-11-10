#ifndef _CS_LB_CHK_USER_S_VO_H_
#define _CS_LB_CHK_USER_S_VO_H_

#include "Type.h"

struct CS_LB_CHK_USER_S_Vo
{
    CS_LB_CHK_USER_S_Vo()
        : LoginKey(0)
        , PLUniID(0)
    {

    }

    Int     LoginKey;   //< LS傳送CL的登入Key
    Dword   PLUniID;    //< 玩家的唯一識別碼(登入失敗時為0)
};

#endif //_CS_LB_CHK_USER_S_VO_H_
