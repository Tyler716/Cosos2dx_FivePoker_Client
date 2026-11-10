#ifndef _GAMESERVERPROXY_HPP_
#define _GAMESERVERPROXY_HPP_
#include "SocketProxy.h"
#include "ConnectionModelData.h"
#include "IData.h"
#include "../ModelData/LoginBufferModelData.h"
#include "../ModelData/PlayerModelData.h"
#include "../ModelData/GameServerLoginBufferModelData.h"
#include "../ModelData/GCliUserInfoListModelData.h"
#include "../ModelData/GameGroupBufferModelData.h"

#include "../../Common/CommandList.h"
#include <string>
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Services::SocketProxy;
        using BaseLayer::Interfaces::INotification;
        using BaseLayer::Common::IData;

        class GameServerProxy : public SocketProxy
        {
        public:
            GameServerProxy(std::string const &proxy_name =  Proxy::NAME)
                : SocketProxy(proxy_name)
            {
                datamap = NULL;

                registerModelData("GameServerLoginBufferModelData", (IData *)new GameServerLoginBufferModelData());

                //TODO: Central¤w¦³µù¥U
                registerModelData("GCliUserInfoListModelData", (IData *)new GCliUserInfoListModelData());
                registerModelData("GameGroupBufferModelData", (IData *)new GameGroupBufferModelData());
            }

            virtual void onRegister(void)
            {
                datamap = (std::map<std::string, IData *> *)_data;

                SocketProxy::onRegister();
            }

            virtual void onRemove(void)
            {
                SocketProxy::onRemove();

                datamap = (std::map<std::string, IData *> *)_data;
                std::map<std::string, IData *> ::iterator iter = datamap->find("ConnectionModelData");

                if(iter != datamap->end())
                {
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"GameServerProxy ConnectionModelData:"<<(ConnectionModelData *)(iter->second)<<std::endl;
#endif
                    if(((ConnectionModelData *)(iter->second))->connectState == ConnectionModelData::CONNECT_SUCCESS)
                    {
                        if(((ConnectionModelData *)(iter->second))->_client->Shutdown())
                        {
                            ((ConnectionModelData *)(iter->second))->connectState = ConnectionModelData::CONNECT_NONE;

#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "connectState set to CONNECT_NONE!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

                            if(((ConnectionModelData *)(iter->second))->_client->Close())
                            {
#ifdef _DEBUG_CONSOLE_
                                std::cout<<"GameServerProxy close socket success!"<<std::endl;
                                std::cout<<"GameServerProxy connectState:"<<((ConnectionModelData *)(iter->second))->connectState<<std::endl;
#endif
                            }
                            else
                            {
#ifdef _DEBUG_CONSOLE_
                                std::cout<<"GameServerProxy close fail!"<<std::endl;
#endif
                            }
                        }
                        else
                        {
#ifdef _DEBUG_CONSOLE_
                            std::cout<<"GameServerProxy shutdown fail!"<<std::endl;
#endif
                        }
                    }
                    else
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"GameServerProxy connectstate is not success!"<<std::endl;
#endif
                    }
                }

                iter = datamap->find("GCliUserInfoListModelData");
                if(iter != datamap->end())
                {
                    GCliUserInfoListModelData *pUserInfoListModelData = (GCliUserInfoListModelData *)iter->second;

                    std::map<int, GCliUserInfo_Vo *>::iterator iter_userinfo = pUserInfoListModelData->UserInfoMap.begin();

                    for(; iter_userinfo != pUserInfoListModelData->UserInfoMap.end(); ++iter_userinfo)
                    {
                        delete iter_userinfo->second;
                    }
                    pUserInfoListModelData->UserInfoMap.clear();
                }

                releaseModelData("GameServerLoginBufferModelData");
                releaseModelData("GCliUserInfoListModelData");
                releaseModelData("GameGroupBufferModelData");
            }

            virtual ~GameServerProxy()
            {
            }

        protected:
            virtual std::string CommandIDToCommandName(int commandID)
            {
                //GameServerEvent_map
                std::map<std::string, int>::iterator iter = GameServerEvent_map.begin();

                for(; iter != GameServerEvent_map.end(); ++iter)
                {
                    if(iter->second == commandID)
                    {
#if defined(_DEBUG_CONSOLE_)
                        if(commandID != 257)
                        {
                            std::cout<<"CommandID:"<<commandID<<" convert to Command:"<<iter->first<<std::endl;
                        }
#endif
                        return iter->first;
                    }
                }

                iter = GameCliMsg_map.begin();
                for(; iter != GameCliMsg_map.end(); ++iter)
                {
                    if(iter->second == commandID)
                    {
#if defined(_DEBUG_CONSOLE_)
                        std::cout<<"CommandID:"<<commandID<<" convert to Command:"<<iter->first<<std::endl;
#endif
                        return iter->first;
                    }
                }

                return "";
            }

        private:
            std::map<std::string, IData *> *datamap;
        };
    }
}

#endif  //_GAMESERVERPROXY_HPP_
