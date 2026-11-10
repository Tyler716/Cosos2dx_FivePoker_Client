#ifndef _GAMESERVERLOGINBUFFERMODELDATA_H_
#define _GAMESERVERLOGINBUFFERMODELDATA_H_

#include "IData.h"
#include "../vo/cs/CS_LB_USER_RELOGIN_GS_R_Vo.h"

using BaseLayer::Common::IData;

struct GameServerLoginBufferModelData : public IData
{
    GameServerLoginBufferModelData()
    {
        memset(&loginData, 0, sizeof(loginData));
    }

    CS_LB_USER_RELOGIN_GS_R_Vo loginData;
};

#endif //_GAMESERVERLOGINBUFFERMODELDATA_H_
