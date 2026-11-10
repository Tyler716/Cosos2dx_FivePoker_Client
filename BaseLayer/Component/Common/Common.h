#ifndef _COMMON_H_
#define _COMMON_H_

#include "InstanceMap.h"
#include "Event.h"
#include "Type.h"
#include "../Interfaces/IObserver.h"
#include "../Interfaces/IModel.h"
#include "../Interfaces/IView.h"
#include "../Interfaces/IController.h"
#include "../Interfaces/IFacade.h"
#include <wchar.h>
/*
#ifndef CC_PLATFORM_ANDROID
#define CC_PLATFORM_ANDROID 2
#endif

#ifndef CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM CC_PLATFORM_ANDROID
#endif
*/
#define SAFE_DELETE(p)  if(p){delete p; p = NULL;}

using BaseLayer::Common::InstanceMap;
using namespace BaseLayer::Interfaces;

static char const _BaseLayer_DEFAULT_NAME_[] = "BaseLayer";
static char const EncrypterKey[] = "www.midnet.com.tw";

extern char const __OBSERVER_NAME__[];
typedef BaseLayer::Common::InstanceMap<IObserver, __OBSERVER_NAME__> InstanceMapObserver;
static InstanceMapObserver observer_instance_map;

extern char const __CONTROLLER_NAME__[];
typedef BaseLayer::Common::InstanceMap<IController, __CONTROLLER_NAME__> InstanceMapController;
static InstanceMapController controller_instance_map;

extern char const __MODEL_NAME__[];
typedef BaseLayer::Common::InstanceMap<BaseLayer::Interfaces::IModel, __MODEL_NAME__> InstanceMapModel;
static InstanceMapModel model_instance_map;

extern char const __VIEW_NAME__[];
typedef BaseLayer::Common::InstanceMap<BaseLayer::Interfaces::IView, __VIEW_NAME__> InstanceMapView;
static InstanceMapView view_instance_map;

extern char const __FACADE_NAME__[];
typedef BaseLayer::Common::InstanceMap<BaseLayer::Interfaces::IFacade, __FACADE_NAME__> InstanceMapFacade;
extern InstanceMapFacade facade_instance_map;
/*
extern char const _Event_NAME_[];
typedef BaseLayer::Common::InstanceMap<int, _Event_NAME_> InstanceMapEvent;
*/
static std::map<std::string, int> LoginServerEvent_map;
static std::map<std::string, int> CentralServerEvent_map;
static std::map<std::string, int> GameServerEvent_map;
static std::map<std::string, int> GameCliMsg_map;
static std::map<std::string, int> GameSLMsg_map;

static const int MAX_ACCOUNT = 22;
static const int MAX_PASSWORD = 22;
static const int MAX_NICKNAME = 18;
static const int MAX_LEVELNAME = 18;
static const int MAX_PIC_LENGTH = 66;
static const int STORE_SOCKET_BUF_LEN = 4194304; //command size 22-bits, total 4MB
static const int RECV_SOCKET_BUF_LEN = 2048; //receive buffer 2k

static const int CT_SYS = 0;
static const int CT_APP = 1;
static const int S2C_SCID_KEEPALIVE = 0;
static const int C2S_SCID_KEEPALIVE_ACK = 1;
static const int C2S_SCID_LOGIN = 2;
static const int S2C_SCID_LOGIN_PERMISSION = 3;
static const int C2S_SCID_LOGOUT = 4;
static const int S2C_SCID_LOGOUT = 5;

#endif //_COMMON_H_
