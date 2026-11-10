#ifndef _LS_LB_CHK_USER_R_VO_H_
#define _LS_LB_CHK_USER_R_VO_H_

#include "CS_ListData.h"

struct LS_LB_CHK_USER_R_Vo
{
    LS_LB_CHK_USER_R_Vo()
        : State(0)
        , LoginKey(0)
        , PLUniID(0)
        , nCSrvs(0)
        //, ListData(NULL)
    {

    }

    Int             State;      ///< LOGIN成功或失敗。0: 正常登入, 1: 無此帳號 + gName, 2: 密碼錯誤, 3: 上層帳號停用, 4: 本身帳號停用, 5: 伺服器維護中, 6: 伺服器全滿
    Int             LoginKey;   ///< 伺服器登入Key
    Dword           PLUniID;    ///< 玩家的唯一識別碼(登入失敗時為0)
    Int             nCSrvs;     ///< central server 數量
    //CS_ListData     *ListData;  ///< 各central server相關資訊

    enum
    {
        STATE_OK = 0,                           //0: 正常登入
        STATE_ACCOUNT_NOT_EXIST = 1,            //無此帳號
        STATE_WRONG_PASSWORD = 2,               //密碼錯誤
        STATE_SUPERIOR_ACCOUNT_DISABLED = 3,    //上層帳號停用
        STATE_ACCOUNT_DISABLED = 4,             //本身帳號停用
        STATE_IP_BANNED = 5,                    //IP 或電腦被鎖掉
        STATE_REPEAT_LOGIN = 6,                 //卡號重覆登入
        STATE_SERVER_FULL = 7,                  //伺服器全滿
        STATE_SERVER_MAINTAINED = 8             //伺服器維護中
    };
};

#endif //_LS_LB_CHK_USER_R_VO_H_
