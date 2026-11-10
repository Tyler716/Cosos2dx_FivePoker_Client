#ifndef _CS_FIVE_POKER_GROUP_H_
#define _CS_FIVE_POKER_GROUP_H_

#include "type.h"

struct CS_FIVE_POKER_GROUP
{
    Int         CSGrpNo;                        //< 局號
    Int         Hold;                           //< 保留桌與否. 0: 不保留, 1: 遊戲中, 2: 他人保留, 3: 本人保留
    Int         MinBuyIn;                       //< 最小BuyIn金額
    Int         MaxBuyIn;                       //< 最大BuyIn金額
    wchar_t     PLNickName[MAX_NICKNAME];       //< 玩家暱稱
    wchar_t     PLPicURL[MAX_PIC_LENGTH];       //< 照片連結網址
    Int         iHour;                          //< 保留的小時
    Int         iMinute;                        //< 保留的分鐘
    Int         iSecond;                        //< 保留的秒數
};

#endif //_CS_FIVE_POKER_GROUP_H_
