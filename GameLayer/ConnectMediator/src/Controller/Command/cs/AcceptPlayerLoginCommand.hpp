#ifndef _ACCEPTPLAYERLOGINCOMMAND_HPP_
#define _ACCEPTPLAYERLOGINCOMMAND_HPP_

#include "Command.h"
#include "../../../Model/vo/cs/CS_LB_CHK_USER_R_Vo.h"
#include "../../../Model/ModelData/PlayerModelData.h"
/******暫時跳過流程所需要的header*******/
#include "Type.h"
#include "../../../Model/vo/cs/CS_LB_USER_JOIN_BUILTIN_GRP_S_Vo.h"
#include "../../../Common/GameProtocol.h"
/**************************************/
#include "../../../Common/CommandList.h"
#include <wchar.h>
#include <stdlib.h>
#include <cstring>

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class AcceptPlayerLoginCommand : public Command
        {
        public:
            AcceptPlayerLoginCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get AcceptPlayerLoginCommand!"<<std::endl;
#endif
                IProxy *CS_proxy = NULL;

                CS_LB_CHK_USER_R_Vo *vo = (CS_LB_CHK_USER_R_Vo *)note.getBody();

                if(vo->State == CS_LB_CHK_USER_R_Vo::STATE_OK)
                {
                    if(getFacade().hasProxy("CentralServerProxy"))
                    {
                        //啟動 ping 發送

                        CS_proxy = &getFacade().retrieveProxy("CentralServerProxy");
                        std::map<std::string, IData *> *CS_datamap = (std::map<std::string, IData *> *)CS_proxy->getData();
                        PlayerModelData *Playermodeldata = (PlayerModelData *)CS_datamap->find("PlayerModelData")->second;

                        //儲存玩家資訊到模型
                        Playermodeldata->UpdatePoint(vo->nFPoints, vo->nVPoints, vo->nWarPoints);
                        Playermodeldata->updateExperience(vo->nExps, vo->nBonus, vo->Level, vo->m_szLevelName);
                        Playermodeldata->PLUniID = vo->PLUniID;
                        Playermodeldata->PointRatio = vo->PointRatio;
                        Playermodeldata->GameTypeFlags = vo->GameTypeFlags;
                        Playermodeldata->GameModeFlasg = vo->GameModeFlasg;

                        wcscpy(Playermodeldata->m_szNickName, vo->m_szNickName);
                        wcscpy(Playermodeldata->m_szPicURL, vo->m_szPicURL);
                        wcscpy(Playermodeldata->m_szAccount, vo->m_szAccount);
                    }

                    sendNotification(CentralServerEvent::CS_LB_GET_GAME_STATE_Event::Name);

                    //TODO:先跳過gamestate...直接做後面的流程
                    if(CS_proxy != NULL)
                    {
                        std::map<std::string, IData *> *CS_datamap = (std::map<std::string, IData *> *)CS_proxy->getData();
                        ConnectionModelData *Connectmodeldata = (ConnectionModelData *)CS_datamap->find("ConnectionModelData")->second;

                        CS_LB_USER_JOIN_BUILTIN_GRP_S_Vo vo;
                        memset(&vo, 0, sizeof(vo));

                        vo.GameType = GAME_TYPE_FIVE_PK;
                        vo.GameMode = GAME_GRP_MODE_AUTO_LV1;
                        vo.CSGrpNo = 32767;
                        vo.SeatNo = 32767;
                        vo.CoinType = COIN_TYPE_V_COIN;
                        vo.KeepTable = 0;
                        vo.GameWindowID = 0;

#ifdef _DEBUG_CONSOLE_
                        std::cout<<"send CS_LB_USER_JOIN_BUILTIN_GRP_S_Vo"<<std::endl;
#endif

                        //join group
                        Connectmodeldata->_client->SendData(CentralServerEvent_map["CS_LB_USER_JOIN_BUILTIN_GRP"],
                            (Byte *)&vo, sizeof(vo));
                    }
                }
                else
                {

                }

                //送出登入結果的訊號，由相關 mediator 接收
                //sendNotification
            }
        };
    }
}

#endif //_ACCEPTPLAYERLOGINCOMMAND_HPP_
