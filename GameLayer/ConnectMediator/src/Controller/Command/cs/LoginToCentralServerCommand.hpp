#ifndef _LOGINTOCENTRALSERVERCOMMAND_HPP_
#define _LOGINTOCENTRALSERVERCOMMAND_HPP_

#include "Command.h"
#include "../../../Model/vo/cs/CS_LB_CHK_USER_S_Vo.h"
#include "../../../Model/ModelData/LoginBufferModelData.h"
#include "../../../Model/ModelData/PlayerModelData.h"
#include "../../../Common/CommandList.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class LoginToCentralServerCommand : public Command
        {
        public:
            LoginToCentralServerCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
                IProxy *CS_proxy = NULL;

#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get LoginToCentralServerCommand!"<<std::endl;
#endif

                if(!getFacade().hasProxy("CentralServerProxy"))
                {
                    //不存在proxy, print error log
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"CentralServerProxy doesn't exist when LoginToCentralServerCommand!"<<std::endl;
#endif
                }
                else    //已存在
                {
                    CS_proxy = &getFacade().retrieveProxy("CentralServerProxy");

                    if(CS_proxy != NULL)
                    {
                        std::map<std::string, IData *> *CS_datamap = (std::map<std::string, IData *> *)CS_proxy->getData();
                        ConnectionModelData *Connectmodeldata = (ConnectionModelData *)CS_datamap->find("ConnectionModelData")->second;
                        PlayerModelData *Playermodeldata = (PlayerModelData *)CS_datamap->find("PlayerModelData")->second;

                        IProxy *LS_proxy = &getFacade().retrieveProxy("LoginServerProxy");
                        std::map<std::string, IData *> *LS_datamap = (std::map<std::string, IData *> *)LS_proxy->getData();
                        LoginBufferModelData *Loginmodeldata = (LoginBufferModelData *)LS_datamap->find("LoginBufferModelData")->second;

                        CS_LB_CHK_USER_S_Vo cslbChkUsrVo;

                        cslbChkUsrVo.LoginKey = Loginmodeldata->centralServerLoginKey;
                        cslbChkUsrVo.PLUniID = Playermodeldata->PLUniID;

                        //TODO:login後等待server回應
                        CheckUser(Connectmodeldata->_client, &cslbChkUsrVo);
                    }
                    else
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"LoginToCentralServerCommand get LoginServerProxy fail!"<<std::endl;
#endif
                    }
                }
            }

        private:
            void CheckUser(GameSocketClient *client, CS_LB_CHK_USER_S_Vo *vo)
            {
                client->SendData(CentralServerEvent_map["CS_LB_CHK_USER"],
                                 (Byte *)vo, sizeof(*vo));
            }

        };
    }
}

#endif //_LOGINTOCENTRALSERVERCOMMAND_HPP_
