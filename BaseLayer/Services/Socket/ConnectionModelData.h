#ifndef _CONNECTIONMODELDATA_H_
#define _CONNECTIONMODELDATA_H_

#include "../../Component/Common/IData.h"
#include "../../Component/Net/GameSocketClient.h"

using BaseLayer::Common::IData;
using BaseLayer::NET::GameSocketClient;

struct ConnectionModelData : public IData
{
    ConnectionModelData()
        : connectState(CONNECT_NONE),
          ip(""),
          port(0),
          autoLogin(false),
          _client(NULL)
    {
    }

    //virtual ~ConnectionModelData(){};

    typedef int STATE;

    enum CONNECT
    {
        CONNECT_NONE,
        CONNECT_SUCCESS,
        CONNECT_FAIL,
        CONNECT_ERROR
    };

    STATE connectState;
    std::string ip;
    int port;
    bool autoLogin;

    GameSocketClient *_client;
};

#endif //_CONNECTIONMODELDATA_H_
