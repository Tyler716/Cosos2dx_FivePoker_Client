#ifndef _CENTRALSERVERPROXY_HPP_
#define _CENTRALSERVERPROXY_HPP_
#include "SocketProxy.h"
#include "ConnectionModelData.h"
#include "IData.h"
#include "../vo/ls/LS_LB_CHK_USER_R_Vo.h"
#include "../vo/cs/CS_LB_CHK_USER_S_Vo.h"
#include "../vo/ls/CS_ListData.h"
#include "../ModelData/BroadcastModelData.h"
#include "../ModelData/ChatModelData.h"
#include "../ModelData/GameGroupBufferModelData.h"
#include "../ModelData/GameInfoManagerModelData.h"
#include "../ModelData/GameWindowInfoModelData.h"
#include "../ModelData/GameWindowManagerModelData.h"
#include "../ModelData/LoginBufferModelData.h"
#include "../ModelData/PlayerModelData.h"
#include "../ModelData/GCliUserInfoListModelData.h"
#include "../ModelData/CSUserInfoListModelData.h"
#include "../../Common/CommandList.h"
#include <string>

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Services::SocketProxy;
        using BaseLayer::Interfaces::INotification;
        using BaseLayer::Common::IData;

        class CentralServerProxy : public SocketProxy
        {
        public:
            CentralServerProxy(std::string const &proxy_name =  Proxy::NAME)
                : SocketProxy(proxy_name)
            {
                datamap = NULL;

                registerModelData("BroadcastModelData", (IData *)new BroadcastModelData());
                registerModelData("ChatModelData", (IData *)new ChatModelData());
                registerModelData("GameGroupBufferModelData", (IData *)new GameGroupBufferModelData());
                registerModelData("GameInfoManagerModelData", (IData *)new GameInfoManagerModelData());
                registerModelData("GameWindowInfoModelData", (IData *)new GameWindowInfoModelData());
                registerModelData("GameWindowInfoModelData", (IData *)new GameWindowInfoModelData());
                registerModelData("PlayerModelData", (IData *)new PlayerModelData());
                registerModelData("GCliUserInfoModelData", (IData *)new GCliUserInfoListModelData());
                registerModelData("CSUserInfoListModelData", (IData *)new CSUserInfoListModelData());
            }

            virtual void onRegister(void)
            {
                datamap = (std::map<std::string, IData *> *)_data;

                SocketProxy::onRegister();

                //TickEnabled(true);
            }

            virtual void onRemove(void)
            {
                SocketProxy::onRemove();

                datamap = (std::map<std::string, IData *> *)_data;
                std::map<std::string, IData *> ::iterator iter = datamap->find("ConnectionModelData");

                if(iter != datamap->end())
                {
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"CentralServerProxy ConnectionModelData:"<<(ConnectionModelData *)(iter->second)<<std::endl;
#endif
                    if(((ConnectionModelData *)(iter->second))->connectState == ConnectionModelData::CONNECT_SUCCESS)
                    {
                        if(((ConnectionModelData *)(iter->second))->_client->Shutdown())
                        {
                            if(((ConnectionModelData *)(iter->second))->_client->Close())
                            {
#ifdef _DEBUG_CONSOLE_
                                std::cout<<"CentralServerProxy close socket success!"<<std::endl;
#endif
                                ((ConnectionModelData *)(iter->second))->connectState = ConnectionModelData::CONNECT_NONE;
#ifdef _DEBUG_CONSOLE_
                                std::cout<<"CentralServerProxy connectState:"<<((ConnectionModelData *)(iter->second))->connectState<<std::endl;
#endif
                            }
                            else
                            {
#ifdef _DEBUG_CONSOLE_
                                std::cout<<"CentralServerProxy close fail!"<<std::endl;
#endif
                            }
                        }
                        else
                        {
#ifdef _DEBUG_CONSOLE_
                            std::cout<<"CentralServerProxy shutdown fail!"<<std::endl;
#endif
                        }
                    }
                    else
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"CentralServerProxy connectstate is not success!"<<std::endl;
#endif
                    }
                }

                releaseModelData("BroadcastModelData");
                releaseModelData("ChatModelData");
                releaseModelData("GameGroupBufferModelData");
                releaseModelData("GameInfoManagerModelData");
                releaseModelData("GameWindowInfoModelData");
                releaseModelData("PlayerModelData");
                releaseModelData("GCliUserInfoModelData");
                releaseModelData("CSUserInfoListModelData");
            }

            virtual ~CentralServerProxy()
            {
            }

        protected:
            virtual std::string CommandIDToCommandName(int commandID)
            {
                //CentralServerEvent_map
            std::map<std::string, int>::iterator iter = CentralServerEvent_map.begin();

            for(; iter != CentralServerEvent_map.end(); ++iter)
            {
                if(iter->second == commandID)
                {
#if defined(_DEBUG_CONSOLE_)
                    if(commandID != 1302)
                    {
                        std::cout<<"CommandID:"<<commandID<<" convert to Command:"<<iter->first<<std::endl;
                    }
#endif
                    return iter->first;
                }
            }
            return NULL;

                return NULL;
            }

        private:
        /*
            void TickEnabled(bool bEnable)
            {
                sigevent sigev;

                if(bEnable)
                {
                    memset(&sigev, 0, sizeof(sigev));
                    sigev.sigev_notify = SIGEV_THREAD;
                    sigev.sigev_notify_function = &CentralServerProxy::onTick;
                    sigev.sigev_value.sival_ptr = this;

                    if(timer_create(CLOCK_REALTIME, &sigev, &tid) < 0)
                    {
                        perror ("timer_creat");
                    }

                    itimerspec ts, ots;
                    memset(&ts, 0, sizeof(ts));
                    memset(&ots, 0, sizeof(ots));

                    ts.it_interval.tv_sec = 5;  //5¬íÄ²µo¤@¦¸
                    ts.it_interval.tv_nsec = 0;
                    ts.it_value = ts.it_interval;

                    timer_settime(tid, TIMER_ABSTIME, &ts, &ots);
                }
                else
                {
                    timer_delete(tid);
                }
            }

            static void onTick(sigval_t v)
            {
                //TODO:
                static_cast<CentralServerProxy *>(v.sival_ptr)->sendNotification("CS_LB_TICK");
            }
*/
            std::map<std::string, IData *> *datamap;

            //timer_t tid;
        };
    }
}

#endif  //_CENTRALSERVERPROXY_HPP_
