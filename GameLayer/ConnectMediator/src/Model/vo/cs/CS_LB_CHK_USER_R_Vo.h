#ifndef _CS_LB_CHK_USER_R_VO_H_
#define _CS_LB_CHK_USER_R_VO_H_

#include "Type.h"

struct CS_LB_CHK_USER_R_Vo
{
    Int         State;                              ///< LOGIN成功或失敗。0:未經由合法的連線過程, 1: 成功, 2: 主機維護中無法登入, 3: 帳號重覆登入
    Dword       PLUniID;                            //< 玩家的唯一識別碼
    Int         nExps;                              //< 玩家的經驗值
    Int         nVPoints;                           //< 玩家的有價點數
    Int         nFPoints;                           //< 玩家的免費點數
    Int         nWarPoints;                         //< 玩家的戰點
    Int         nBonus;                             //< 玩家的累積紅利
    Int         Level;                              //< 玩家的等級
    Int         PointRatio;                         //< 點數比值
    Dword       GameTypeFlags;                      //< 可執行的遊戲類型
    Dword       GameModeFlasg;                      //< 可執行的遊戲模式
    wchar_t     m_szAccount[MAX_ACCOUNT];           //< 玩家帳號
    wchar_t     m_szNickName[MAX_NICKNAME];         //< 玩家暱稱
    wchar_t     m_szLevelName[MAX_LEVELNAME];       //< 等級名稱
    wchar_t     m_szPicURL[MAX_PIC_LENGTH];         //< 照片連結網址

    enum
    {
        STATE_ILLEGAL_CONNECT = 0,      //0:未經由合法的連線過程
        STATE_OK = 1,                   //1: success
        STATE_SERVER_MAINTAINED = 2,    //2: 主機維護中無法登入
        STATE_REPEAT_LOGIN = 3          //3: 帳號重覆登入
    };
};

#endif //_CS_LB_CHK_USER_R_VO_H_
