#ifndef _CENTRALSERVERTICKCOMMAND_HPP_
#define _CENTRALSERVERTICKCOMMAND_HPP_

#include "Command.h"
#include "ConnectionModelData.h"
//#include "../../../Model/vo/cs/CS_LB_GET_USR_LIST_S_Vo.h"
#include "../../../Common/CommandList.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class CentralServerTickCommand : public Command
        {
        public:
            CentralServerTickCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
                IProxy *CS_proxy = NULL;
                if(!getFacade().hasProxy("CentralServerProxy"))
                {
                    //不存在proxy, print error log
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"CentralServerProxy doesn't exist when CentralServerTickCommand!"<<std::endl;
#endif
                }
                else    //已存在
                {
                    CS_proxy = &getFacade().retrieveProxy("CentralServerProxy");

                    if(CS_proxy != NULL)
                    {
                        std::map<std::string, IData *> *CS_datamap = (std::map<std::string, IData *> *)CS_proxy->getData();
                        ConnectionModelData *Connectmodeldata = (ConnectionModelData *)CS_datamap->find("ConnectionModelData")->second;

                        Connectmodeldata->_client->SendData(CentralServerEvent_map["CS_LB_TICK"], NULL, 0);
                        /*
                        static int iNum = 0;

                        ++iNum;

                        if(iNum == 1)
                        {
                            //getFacade().sendNotification(CentralServerEvent::CS_LB_GET_USR_LIST_Event::Name);
                            CS_LB_GET_USR_LIST_S_Vo vo;
                            vo.QueryType = 1;
                            vo.UsrFmNo = 0;
                            vo.UsrEdNo = 1000;
                            Connectmodeldata->_client->SendData(CentralServerEvent_map["CS_LB_GET_USR_LIST"], (Byte *)&vo, sizeof(vo));
                        }
                        else if(iNum == 50)
                        {
                            iNum = 0;
                        }*/
                    }
                }
            }
        };
    }
}

#endif //_CENTRALSERVERTICKCOMMAND_HPP_
