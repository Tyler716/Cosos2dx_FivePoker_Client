#ifndef _LOGINSERVERPROXY_HPP_
#define _LOGINSERVERPROXY_HPP_
#include "SocketProxy.h"
#include "ConnectionModelData.h"
#include "IData.h"
#include "../ModelData/LoginBufferModelData.h"
#include "../../Common/CommandList.h"
#include <string>

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Services::SocketProxy;
        using BaseLayer::Interfaces::INotification;
        using BaseLayer::Common::IData;

        class LoginServerProxy : public SocketProxy
        {
        public:
            LoginServerProxy(std::string const &proxy_name =  Proxy::NAME)
                : SocketProxy(proxy_name)
            {
                datamap = NULL;

                registerModelData("LoginBufferModelData", (IData *)new LoginBufferModelData());
            }

            virtual void onRegister(void)
            {
                datamap = (std::map<std::string, IData *> *)_data;

                ConnectionModelData *modeldata = (ConnectionModelData *)datamap->find("ConnectionModelData")->second;

                if(modeldata != NULL)
                {
                    if(modeldata->connectState == ConnectionModelData::CONNECT_SUCCESS)
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"LoginServerProxy connect success!"<<std::endl;
#endif
                    }
                }

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
                    std::cout<<"LoginServerProxy ConnectionModelData:"<<(ConnectionModelData *)(iter->second)<<std::endl;
#endif
                    if(((ConnectionModelData *)(iter->second))->connectState == ConnectionModelData::CONNECT_SUCCESS)
                    {
                        if(((ConnectionModelData *)(iter->second))->_client->Shutdown())
                        {
                            if(((ConnectionModelData *)(iter->second))->_client->Close())
                            {
#ifdef _DEBUG_CONSOLE_
                                std::cout<<"LoginServerProxy close socket success!"<<std::endl;
#endif
                                ((ConnectionModelData *)(iter->second))->connectState = ConnectionModelData::CONNECT_NONE;
#ifdef _DEBUG_CONSOLE_
                                std::cout<<"LoginServerProxy connectState:"<<((ConnectionModelData *)(iter->second))->connectState<<std::endl;
#endif
                            }
                            else
                            {
#ifdef _DEBUG_CONSOLE_
                                std::cout<<"LoginServerProxy close fail!"<<std::endl;
#endif
                            }
                        }
                        else
                        {
#ifdef _DEBUG_CONSOLE_
                            std::cout<<"LoginServerProxy shutdown fail!"<<std::endl;
#endif
                        }
                    }
                    else
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"LoginServerProxy connectstate is not success!"<<std::endl;
#endif
                    }
                }

                releaseModelData("LoginBufferModelData");
            }

            void sendNotification(std::string const &notification_name,
                                  void const *body /*= NULL*/,
                                  std::string const &type /*= ""*/)
            {
                Notifier::sendNotification(notification_name, body, type);
            }

            virtual ~LoginServerProxy()
            {
            }

        protected:
            virtual std::string CommandIDToCommandName(int commandID)
            {
                std::map<std::string, int>::iterator iter = LoginServerEvent_map.begin();

                for(; iter != LoginServerEvent_map.end(); ++iter)
                {
                    if(iter->second == commandID)
                    {
                        return iter->first;
                    }
                }
                return NULL;
            }

        private:
            std::map<std::string, IData *> *datamap;
        };
    }
}
#endif //_LOGINSERVERPROXY_HPP_
