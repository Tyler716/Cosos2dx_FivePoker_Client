#ifndef _ACCEPTUSERLISTCOMMAND_HPP_
#define _ACCEPTUSERLISTCOMMAND_HPP_

#include "Command.h"
#include "../../../Common/CommandList.h"
#include "../../../Model/vo/cs/CS_LB_GET_USR_LIST_R_Vo.h"
#include "../../../Model/ModelData/CSUserInfoListModelData.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class AcceptUserListCommand : public Command
        {
        public:
            AcceptUserListCommand()
                : Command()
            {
                m_pCSUserInfoModelData = NULL;
            }

            virtual void execute(INotification const &note)
            {
                CS_LB_GET_USR_LIST_R_Vo *vo = (CS_LB_GET_USR_LIST_R_Vo *)note.getBody();

                if(vo)
                {
                    if(!m_pCSUserInfoModelData)
                    {
                        IProxy *CS_proxy = NULL;

                        if(getFacade().hasProxy("CentralServerProxy"))
                        {
                            //啟動 ping 發送

                            CS_proxy = &getFacade().retrieveProxy("CentralServerProxy");
                            std::map<std::string, IData *> *CS_datamap = (std::map<std::string, IData *> *)CS_proxy->getData();
                            m_pCSUserInfoModelData = (CSUserInfoListModelData *)CS_datamap->find("CSUserInfoListModelData")->second;
                        }
                        else    //no CentralServerProxy
                        {
                            //error
                        }
                    }

                    if(m_pCSUserInfoModelData)  //update userinfo model data
                    {
                        memset(m_pCSUserInfoModelData->UserInfo, 0, sizeof(m_pCSUserInfoModelData->UserInfo));

                        CS_USER_INFO *userinfo = NULL;

                        if(sizeof(m_pCSUserInfoModelData->UserInfo)/sizeof(m_pCSUserInfoModelData->UserInfo[0]) >= vo->nTotalUsers)
                        {
                            m_pCSUserInfoModelData->iUserNum = vo->nTotalUsers;

                            for(int i = 0; i < vo->nTotalUsers; i++)
                            {
                                userinfo = (CS_USER_INFO *)(((Byte *)vo) + sizeof(CS_LB_GET_USR_LIST_R_Vo) + i*sizeof(CS_USER_INFO));
                                memcpy((void *)(m_pCSUserInfoModelData->UserInfo+i), (void *)userinfo, sizeof(CS_USER_INFO));
                            }
                        }
                        else    //model data的userinfo欄位不夠
                        {
                            //error
                        }
                    }
                }
                else
                {
                    //error
                }
            }

        private:
            CSUserInfoListModelData *m_pCSUserInfoModelData;
        };
    }
}

#endif //_ACCEPTUSERLISTCOMMAND_HPP_
