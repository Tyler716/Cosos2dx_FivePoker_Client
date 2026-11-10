#ifndef _GAMEGROUPBUFFERMODELDATA_H_
#define _GAMEGROUPBUFFERMODELDATA_H_

#include "Type.h"
#include "IData.h"

using BaseLayer::Common::IData;

struct GameGroupBufferModelData : public IData
{
    GameGroupBufferModelData()
    {
        m_MenInGrp = 0;
    }

    Int m_MenInGrp;
};

#endif //_GAMEGROUPBUFFERMODELDATA_H_
