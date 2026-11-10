#ifndef _CSUSERINFOLISTMODELDATA_H_
#define _CSUSERINFOLISTMODELDATA_H_

#include "Type.h"
#include "IData.h"
#include "../vo/cs/CS_LB_GET_USR_LIST_R_Vo.h"

using BaseLayer::Common::IData;

struct CSUserInfoListModelData : public IData
{
    CSUserInfoListModelData()
    {
        memset(UserInfo, 0, sizeof(UserInfo));
        iUserNum = 0;
    }

    int iUserNum;
    CS_USER_INFO UserInfo[1001];    //key: ?
};

#endif  //_CSUSERINFOLISTMODELDATA_H_