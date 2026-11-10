#ifndef _SENDMSGCOMMAND_HPP_
#define _SENDMSGCOMMAND_HPP_

#include "Command.h"
#include "CommandList.h"
#include "ConnectionModelData.h"
#include "../../Model/Vo/GSLMSG_Vo.h"

namespace GameLayer
{
    namespace Game
    {
        using BaseLayer::Patterns::Command;

        class SendMsgCommand : public Command
        {
        public:
            SendMsgCommand() : Command()
                , m_pGSClient(NULL)
                , m_pGSProxy(NULL)
            {

            }

            virtual void execute(INotification const &note)
            {
                
                if(!m_pGSProxy)
                {
                    if(!getFacade().hasProxy("GameServerProxy"))
                    {
                        //不存在proxy, print error log
    #ifdef _DEBUG_CONSOLE_
                        std::cout<<"GameServerProxy doesn't exist when GameServerTickCommand!"<<std::endl;
    #endif
                    }
                    else    //已存在
                    {
                        m_pGSProxy = &getFacade().retrieveProxy("GameServerProxy");

                        if(m_pGSProxy)
                        {
                            if(!m_pGSClient)
                            {
                                std::map<std::string, IData *> *GS_datamap = (std::map<std::string, IData *> *)m_pGSProxy->getData();
                                ConnectionModelData *Connectmodeldata = (ConnectionModelData *)GS_datamap->find("ConnectionModelData")->second;

                                m_pGSClient = Connectmodeldata->_client;
                            }
                        }
                    }
                }
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get SendMsgCommand!"<<std::endl;
#endif
                if(m_pGSClient)
                {
                    switch(CommandNameToCommandID(note.getName()))
                    {
                        case GameSLMsg::GSLMSG_BET::id:
                        {
                            sendMessage(GameSLMsg::GSLMSG_BET::id,
                                        (Byte *)note.getBody(),
                                        sizeof(GSLBet_Vo));
                            break;
                        }

                        case GameSLMsg::GSLMSG_DRAW::id:
                        {
                            sendMessage(GameSLMsg::GSLMSG_DRAW::id,
                                        (Byte *)note.getBody(),
                                        sizeof(GSLDraw_Vo));
                            break;
                        }

                        case GameSLMsg::GSLMSG_SET_MULTIPLE::id:
                        {
                            sendMessage(GameSLMsg::GSLMSG_SET_MULTIPLE::id,
                                        (Byte *)note.getBody(),
                                        sizeof(GSLSetMultiple_Vo));
                            break;
                        }

                        case GameSLMsg::GSLMSG_PLAYER_DOUBLE_GAME::id:
                        {
                            sendMessage(GameSLMsg::GSLMSG_PLAYER_DOUBLE_GAME::id, NULL, 0);
                            break;
                        }

                        case GameSLMsg::GSLMSG_DOUBLE_GAME::id:
                        {
                            sendMessage(GameSLMsg::GSLMSG_DOUBLE_GAME::id,
                                        (Byte *)note.getBody(),
                                        sizeof(GSLDoubleGame_Vo));
                            break;
                        }

                        case GameSLMsg::GSLMSG_GET_POINT::id:
                        {
                            sendMessage(GameSLMsg::GSLMSG_GET_POINT::id, NULL, 0);
                            break;
                        }

                        case GameClientServiceEvent::SLMSG_GAME_CONTINUE_Event::id:
                        {
                            sendMessage(GameClientServiceEvent::SLMSG_GAME_CONTINUE_Event::id, 
                                        (Byte *)note.getBody(),
                                        sizeof(GSLGameContinue_Vo));
                            break;
                        }

                        case GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::id:
                        {
                            sendMessage(GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::id, 
                                        (Byte *)note.getBody(),
                                        sizeof(GSLWaitSync_Vo));
                            break;
                        }
                    }
                }
            }

        private:
            void sendMessage(int id, Byte *buf, int iSize)
            {
                m_pGSClient->SendData(id, buf, iSize);
            }

            int CommandNameToCommandID(std::string strCmd)
            {
                if(GameSLMsg_map.find(strCmd) != GameSLMsg_map.end())
                {
                    return GameSLMsg_map[strCmd];
                }

                return 0;
            }

        private:
            IProxy              *m_pGSProxy;
            GameSocketClient    *m_pGSClient;
        };
    }
}

#endif