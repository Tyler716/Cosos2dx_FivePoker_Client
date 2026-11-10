#ifndef _LOGINTOLOGINSERVERCOMMAND_HPP_
#define _LOGINTOLOGINSERVERCOMMAND_HPP_

#include "Command.h"
#include "Proxy.h"
#include "Facade.h"
#include "../../../Model/Proxy/LoginServerProxy.hpp"
#include "../../../Model/vo/ls/LoginRequestVo.h"
#include "../../../Common/CommandList.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Patterns::Facade;
        using BaseLayer::Interfaces::INotification;
        using BaseLayer::Interfaces::IFacade;
        using BaseLayer::NET::GameSocketClient;

        class LoginToLoginServerCommand : public Command
        {
        public:
            LoginToLoginServerCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
                //TODO:記住登入資訊, 加入model
                IProxy *proxy = NULL;

#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get LoginToLoginServerCommand!"<<std::endl;
#endif

                if(!getFacade().hasProxy("LoginServerProxy"))
                {
                    //不存在proxy, print error log
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"LoginServerProxy doesn't exist when LoginToLoginServerCommand!"<<std::endl;
#endif
                }
                else    //已存在
                {
                    proxy = &getFacade().retrieveProxy("LoginServerProxy");

                    if(proxy != NULL)
                    {
                        LoginRequestVo *vo = (LoginRequestVo *)note.getBody();

                        if(vo->isLegalRequest())
                        {
                            std::map<std::string, IData *> *datamap = (std::map<std::string, IData *> *)proxy->getData();
                            /*
                            LoginRequestVo *loginReqVo = (LoginRequestVo *)datamap->find("LoginRequestVo")->second;

                            wcscpy(loginReqVo->m_szAccount, vo->m_szAccount);
                            wcscpy(loginReqVo->m_szPassword, vo->m_szPassword);*/

                            ConnectionModelData *Connect_modeldata = (ConnectionModelData *)datamap->find("ConnectionModelData")->second;

#ifdef _DEBUG_CONSOLE_
                            std::cout<<"Login!"<<std::endl;
                            std::cout<<"size LoginRequestVo:"<<sizeof(LoginRequestVo)<<std::endl;
#endif

                            LoginBufferModelData *LS_modeldata = (LoginBufferModelData *)datamap->find("LoginBufferModelData")->second;

                            wcscpy(LS_modeldata->m_szAccount, vo->m_szAccount);
                            wcscpy(LS_modeldata->m_szPassword, vo->m_szPassword);

                            //TODO:login後等待server回應
                            Login(Connect_modeldata->_client, vo);
                        }
                        else
                        {
                            //TODO:送訊息給畫面通知格式錯誤
                        }
                    }
                    else
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"LoginToLoginServerCommand get LoginServerProxy fail!"<<std::endl;
#endif
                    }
                }
            }

        private:
            void Login(GameSocketClient *client, LoginRequestVo *loginVo)
            {
                client->SendData(LoginServerEvent_map["LS_LB_CHK_USER"],
                                 (Byte *)loginVo, sizeof(*loginVo));
            }
        };
    }
}

#endif //_LOGINTOLOGINSERVERCOMMAND_HPP_
