#ifndef _CS_LISTDATA_VO_H_
#define _CS_LISTDATA_VO_H_

#include "Type.h"
#include <wchar.h>

struct CS_ListData
{
    //Central server是否已滿
    int m_isFull;

    //Central server內玩家人數是否已滿
    int m_userCount;

    //Central server埠號
    int m_serverPort;

    //Central server IP
    STR16 m_serverIP;

    //Central server名稱
    STR16 m_serverName;
};

#endif //_CS_LIST_DATA_VO_H_
