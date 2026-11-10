#ifndef _GCLIUSERINFOLISTMODELDATA_H_
#define _GCLIUSERINFOLISTMODELDATA_H_

#include "Type.h"
#include "IData.h"
#include "../vo/ga/SLMSG_Vo.h"

using BaseLayer::Common::IData;

struct GCliUserInfoListModelData : public IData
{
    GCliUserInfoListModelData()
    {
    }

    virtual ~GCliUserInfoListModelData()
    {
        int i = 0;
        
        std::map<int, GCliUserInfo_Vo *>::iterator iter = UserInfoMap.begin();

        for(; iter != UserInfoMap.end(); ++iter)
        {
            delete iter->second;
        }

        UserInfoMap.clear();
    }

    std::map<int, GCliUserInfo_Vo *> UserInfoMap;    //key: userIndex
};

#endif //_GCLIUSERINFOLISTMODELDATA_H_
