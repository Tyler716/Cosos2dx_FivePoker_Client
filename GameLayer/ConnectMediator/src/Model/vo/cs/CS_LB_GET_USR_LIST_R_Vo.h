#ifndef _CS_LB_GET_USR_LIST_R_VO_H_
#define _CS_LB_GET_USR_LIST_R_VO_H_

#include "Type.h"

struct CS_LB_GET_USR_LIST_R_Vo
{
    Int         UsrFmNo;            ///< 玩家查詢的索引啟始號
    Int         nUsrs;              ///< 玩家數(The number of player to query)
    Int         nTotalUsers;        ///< 玩家總數(The number of player in the CS)
    //USER_INFO UsrLst[];           ///< 玩家清單
};

struct CS_USER_INFO
{
    Dword       PLUniID;                    ///< 玩家的唯一識別碼
    Int         GameTitle;                  ///< 玩家所在遊戲
    Int         Level;                      ///< 玩家的等級
    wchar_t     NickName[MAX_NICKNAME];     ///< 暱稱
    wchar_t     LevelName[MAX_LEVELNAME];   ///< 玩家的等級名稱
    wchar_t     PicURL[MAX_PIC_LENGTH];     ///< 照片連結網址
};

#endif  //_CS_LB_GET_USR_LIST_R_VO_H_