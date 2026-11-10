#ifndef _CONNECTTOCENTRALSERVVERCOMMAND_HPP_
#define _CONNECTTOCENTRALSERVVERCOMMAND_HPP_

#include "Command.h"
#include "Facade.h"
#include "IFacade.h"
#include "../../../Common/CommandList.h"
#include "../../../Model/Proxy/CentralServerProxy.hpp"
#include "../../../Model/vo/ls/LS_LB_CHK_USER_R_Vo.h"
#include "../../../Model/vo/cs/CS_LB_CHK_USER_S_Vo.h"
#include <wchar.h>
#include <stdlib.h>
#include <cstring>

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Patterns::Facade;
        using BaseLayer::Interfaces::INotification;
        using BaseLayer::Interfaces::IFacade;

        class ConnectToCentralServerCommand : public Command
        {
        public:
            ConnectToCentralServerCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
                std::cout<<"get ConnectToCentralServerCommand!"<<std::endl;

                IProxy *CS_proxy = NULL;

                LS_LB_CHK_USER_R_Vo *vo = (LS_LB_CHK_USER_R_Vo *)note.getBody();

                if(!getFacade().hasProxy("CentralServerProxy"))
                {
                    CS_proxy = new CentralServerProxy("CentralServerProxy");
                    std::map<std::string, IData *> *Cs_datamap = (std::map<std::string, IData *> *)CS_proxy->getData();
                    PlayerModelData *Playermodeldata = (PlayerModelData *)Cs_datamap->find("PlayerModelData")->second;

                    IProxy *LSproxy = &getFacade().retrieveProxy("LoginServerProxy");
                    std::map<std::string, IData *> *Ls_datamap = (std::map<std::string, IData *> *)LSproxy->getData();
                    LoginBufferModelData *LSmodeldata = (LoginBufferModelData *)Ls_datamap->find("LoginBufferModelData")->second;

                    if(!LSmodeldata)
                    {
                        //TODO: error log
                    }

                    if(vo)
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"ConnectToCentralServerCommand get valid vo!"<<std::endl;
#endif

                        CS_ListData *serverData = NULL;

                        LSmodeldata->centralServerLoginKey = vo->LoginKey;
                        Playermodeldata->PLUniID = vo->PLUniID;
                        //lblsChkUsrVo->ListData = vo->ListData;

#ifdef _DEBUG_CONSOLE_
                        std::cout<<"State:"<<vo->State
                                 <<", LoginKey:"<<vo->LoginKey
                                 <<", PLUniID:"<<vo->PLUniID
                                 <<", nCSrvs:"<<vo->nCSrvs
                                 <<std::endl;
#endif
#if 1
                        if (vo->State == LS_LB_CHK_USER_R_Vo::STATE_OK)
                        {
                            if(vo->nCSrvs > 0)
                            {
                                //挑選可登入的 central server
                                for(int i = 0; i < vo->nCSrvs; i++)
                                {
                                    serverData = (CS_ListData *)(((Byte *)vo) + sizeof(LS_LB_CHK_USER_R_Vo) + i*sizeof(CS_ListData));

                                    if(serverData != NULL && serverData->m_isFull == false)
                                    {
                                        break;
                                    }
                                }

#ifdef _DEBUG_CONSOLE_
                                std::cout<<"get valid central server!"<<std::endl;
#endif

                                //Login server 連接無誤後斷開LoginServer，轉 central server
                                ConnectionModelData *LS_ConnectionModelData = (ConnectionModelData *)(Ls_datamap->find("ConnectionModelData")->second);

                                if(!LS_ConnectionModelData)
                                {
                                    //TODO: print Error Log
#ifdef _DEBUG_CONSOLE_
                                    std::cout<<"get invalid LS_ConnectionModelData!"<<std::endl;
#endif
                                    return;
                                }

                                if(DisConnectLoginServer(LS_ConnectionModelData))
                                {
#ifdef _DEBUG_CONSOLE_
                                    std::cout<<"DisConnectLoginServer done!"<<std::endl;
#endif
                                    ConnectionModelData *CS_ConnectionModelData = (ConnectionModelData *)(Cs_datamap->find("ConnectionModelData")->second);

                                    if(!CS_ConnectionModelData)
                                    {
                                        //TODO: print Error Log
#ifdef _DEBUG_CONSOLE_
                                        std::cout<<"get invalid CS_sockvo!"<<std::endl;
#endif
                                        return;
                                    }
                                    else
                                    {
                                        if(!serverData)
                                        {
                                            //TODO:print error log
#ifdef _DEBUG_CONSOLE_
                                            std::cout<<"get invalid serverData!"<<std::endl;
#endif
                                            return;
                                        }

#ifdef _DEBUG_CONSOLE_
                                        std::cout<<"serverData with m_isFull:"<<serverData->m_isFull
                                                    <<", m_userCount:"<<serverData->m_userCount
                                                    <<", port:"<<serverData->m_serverPort<<std::endl;
                                        std::wcout<<", ip:"<<serverData->m_serverIP
                                                    <<", m_serverName:"<<serverData->m_serverName
                                                    <<std::endl;
#endif

                                        char ip[16];
                                        memset(ip, 0, sizeof(ip));
                                        wcstombs(ip, serverData->m_serverIP, wcslen(serverData->m_serverIP));

                                        CS_ConnectionModelData->ip = ip;
                                        CS_ConnectionModelData->port = serverData->m_serverPort;
                                    }

                                    if(ConnectCentralServer(CS_ConnectionModelData))
                                    {
                                        getFacade().registerProxy(CS_proxy);
                                        sendNotification(CentralServerConnect);
                                    }
                                    else
                                    {
#ifdef _DEBUG_CONSOLE_
                                        std::cout<<"Connect To Central server fail!"<<std::endl;
#endif
                                    }
                                }
                                else    //disconnect login server fails
                                {
                                    //連線失敗就刪除proxy
                                    delete getFacade().removeProxy("CentralServerProxy");
                                    //delete CS_proxy;

                                    //TODO: 判斷失敗要怎麼處理, 重新login?

                                    //TODO:print error log
                                    //發送登入失敗event
                                }
                            }
                            else   //if(lblsChkUsrVo->nCSrvs > 0) else; central server數量為0
                            {
                                //TODO:print error Log
                            }
                        }
                        else
                        {
                            //送出登入結果的訊號，由相關 mediator 接收
                            getFacade().sendNotification("GAMEMSG_LOGIN_FAIL", &vo->State);
                        }
#endif
                    }
                    else    //if(vo) else; 取不到vo
                    {
                        //TODO: print Error log
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"ConnectToCentralServerCommand get invalid vo!"<<std::endl;
#endif
                    }
                }
                else    //proxy已經存在, 判斷是否重連?
                {

                }
            }

        private:
            bool DisConnectLoginServer(ConnectionModelData *modeldata)
            {
                if(modeldata->_client)
                {
                    if(modeldata->connectState == ConnectionModelData::CONNECT_SUCCESS)
                    {
                        if(modeldata->_client->Shutdown())
                        {
                            if(modeldata->_client->Close())
                            {
                                modeldata->connectState = ConnectionModelData::CONNECT_NONE;

                                return true;
                            }
                        }
                    }
                }

                return false;
            }

            bool ConnectCentralServer(ConnectionModelData *modeldata)
            {
                if(!modeldata->_client)
                {
                    modeldata->_client = new GameSocketClient(modeldata->ip, modeldata->port);

#ifdef _DEBUG_CONSOLE_
                    std::cout<<"ConnectCentralServer _client is NULL, new a client with ip:"<<modeldata->ip<<", port:"<<modeldata->port<<std::endl;
#endif

                    if(modeldata->_client)
                    {
                        if(Connect(modeldata))
                        {
                            return true;
                        }
                    }
                }
                return false;
            }

            bool Connect(ConnectionModelData *modeldata)
            {
                if(!modeldata->_client->Connect())
                {
                    modeldata->connectState = ConnectionModelData::CONNECT_FAIL;
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"ConnectToCentralServerCommand connect fail!"<<std::endl;
#endif
                    return false;
                }
                else
                {
                    modeldata->connectState = ConnectionModelData::CONNECT_SUCCESS;
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"ConnectToCentralServerCommand connect success!"<<std::endl;
#endif
                }
                return true;
            }
        };
    }
}

#endif //_CONNECTTOCENTRALSERVVERCOMMAND_HPP_
