#ifndef _CONNECTTOLOGINSERVERCOMMAND_HPP_
#define _CONNECTTOLOGINSERVERCOMMAND_HPP_

#include "Command.h"
#include "Proxy.h"
#include "SocketVo.h"
#include "../../../Model/Proxy/LoginServerProxy.hpp"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;
        using BaseLayer::Interfaces::IProxy;
        using GameLayer::Lobby::LoginServerProxy;

        class ConnectToLoginServerCommand : public Command
        {
        public:
            ConnectToLoginServerCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
                IProxy *LS_Proxy = NULL;

                SocketVo *vo = (SocketVo *)note.getBody();

                if(!getFacade().hasProxy("LoginServerProxy"))
                {
                    LS_Proxy = new LoginServerProxy("LoginServerProxy");

                    std::map<std::string, IData *> *datamap = (std::map<std::string, IData *> *)LS_Proxy->getData();

                    ConnectionModelData *modeldata = (ConnectionModelData *)datamap->find("ConnectionModelData")->second;

                    if(modeldata)
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"ConnectToLoginServerCommand vo:"<<modeldata<<std::endl;
#endif
                        if(vo)
                        {
                            modeldata->ip = vo->ip;
                            modeldata->port = vo->port;
                            modeldata->autoLogin = vo->autoLogin;
                        }
                        else
                        {
                            modeldata->ip = "127.0.0.1";
                            modeldata->port = 35163;
                            modeldata->autoLogin = true;
                        }

                        if(!modeldata->_client)
                        {
#ifdef _DEBUG_CONSOLE_
                            std::cout<<"ConnectToLoginServerCommand _client is NULL!"<<std::endl;
#endif
                            modeldata->_client = new GameSocketClient(modeldata->ip, modeldata->port);
                        }

                        if(Connect(modeldata))
                        {
                            getFacade().registerProxy(LS_Proxy);

                            getFacade().sendNotification("GAMEMSG_CONNECT_LS_OK");
                        }
                        else
                        {
                            //連線失敗就刪除proxy
                            delete getFacade().removeProxy("LoginServerProxy");
                            //delete LS_Proxy;
                        }
                    }
                    else
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"ConnectToLoginServerCommand vo is NULL!"<<std::endl;
#endif
                    }
                }
                else    //檢查是否斷線要重連
                {
                    LS_Proxy = &getFacade().retrieveProxy("LoginServerProxy");

                    std::map<std::string, IData *> *datamap = (std::map<std::string, IData *> *)LS_Proxy->getData();
                    ConnectionModelData *modeldata = (ConnectionModelData *)datamap->find("ConnectionModelData")->second;

                    bool bReConnect = false;

                    if(!modeldata->_client)
                    {
                        modeldata->_client = new GameSocketClient(modeldata->ip, modeldata->port);

                        bReConnect = true;
                    }
                    else if(modeldata->connectState != ConnectionModelData::CONNECT_SUCCESS)
                    {
                        bReConnect = true;
                    }

                    if(bReConnect)  //因為socketProxy的接收thread在斷線會結束
                    {
                        if(Connect(modeldata))
                        {
                            LS_Proxy->onRegister();    //重跑一次thread
                        }
                    }
                    else
                    {
                        //回報連線成功
                        getFacade().sendNotification("GAMEMSG_CONNECT_LS_OK");
                    }
                }
            }

        private:
            bool Connect(ConnectionModelData *modeldata)
            {
                if(!modeldata->_client->Connect())
                {
                    modeldata->connectState = ConnectionModelData::CONNECT_FAIL;
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"ConnectToLoginServerCommand connect fail!"<<std::endl;
#endif
                    return false;
                }
                else
                {
                    modeldata->connectState = ConnectionModelData::CONNECT_SUCCESS;
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"ConnectToLoginServerCommand connect success!"<<std::endl;
#endif
                }
                return true;
            }
        };
    }
}

#endif //_CONNECTTOLOGINSERVERCOMMAND_HPP_
