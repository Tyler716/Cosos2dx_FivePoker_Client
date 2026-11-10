#ifndef _WAITSYNCCOMMAND_HPP_
#define _WAITSYNCCOMMAND_HPP_

//#include "CommandList.h"
#include "../../Model/vo/GameMsg.h"
#include "../../Model/ModelData/FivePokerModelData.h"
#include "ga/SyncGameServerDataCommand.hpp"
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

namespace GameLayer
{
    namespace Game
    {
        using GameLayer::Lobby::SyncGameServerDataCommand;

        class WaitSyncCommand : public SyncGameServerDataCommand
        {
        public:
            WaitSyncCommand()
            {
                if(getFacade().hasProxy("GameProxy"))
                {
                    IProxy *gameProxy = &getFacade().retrieveProxy("GameProxy");
                
                    std::map<std::string, IData *> *Game_datamap = (std::map<std::string, IData *> *)gameProxy->getData();
                    m_fivePKModelData = (FivePokerModelData *)Game_datamap->find("FivePokerModelData")->second;
                }
            };

            virtual ~WaitSyncCommand(){};

            virtual void execute(INotification const &note)
            {
                int cmdId = GameServerEvent_map[note.getName()];

                switch(cmdId)
                {
                    case GameClientServiceEvent::CLIMSG_USER_INFO_Event::id:  //CLIMSG_USER_INFO_Event
                    {
                        GCliUserInfo_Vo *vo = (GCliUserInfo_Vo *)note.getBody();
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"Get CLIMSG_USER_INFO_Event! user:"<<vo->szUserName<<std::endl;
#endif
                        //m_fivePKModelData->m_uUserIndex = vo->uUserIndex;
                        //m_fivePKModelData->m_uTotalPoints = vo->uTotalPoints;

                        if(vo->uUserIndex >= 0 && m_userInfoListModelData)
                        {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Get userinfo name:%s index:%i...", vo->szUserName, vo->uUserIndex);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                            memcpy(m_userInfoListModelData->UserInfoMap[vo->uUserIndex], vo, sizeof(GCliUserInfo_Vo));
                        }
                        break;
                    }
                }

                SyncGameServerDataCommand::execute(note);
            }

            virtual void waitSync(int SyncId, Byte *data)
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get syncCommand with syncID:"<<SyncId<<std::endl;
#endif
                switch(SyncId)
                {
                    case SYNCSIGNAL_GAME:   //等待至遊戲幕
                    {
                        m_fivePKModelData->m_iGameScene = FivePokerModelData::GAME_SCENE;
                        /*
                        if(!m_fivePKModelData->m_bFirstLoading)
                        {*/
                            GCliWSGame_Vo *vo = (GCliWSGame_Vo *)data;

                            memcpy(m_fivePKModelData->m_GameScore_1, 
                                   vo->GameScore_1, 
                                   sizeof(m_fivePKModelData->m_GameScore_1));
                            memcpy(m_fivePKModelData->m_GameScore_2, 
                                   vo->GameScore_2, 
                                   sizeof(m_fivePKModelData->m_GameScore_2));
                            memcpy(m_fivePKModelData->m_GameScore_3, 
                                   vo->GameScore_3, 
                                   sizeof(m_fivePKModelData->m_GameScore_3));
                            memcpy(m_fivePKModelData->m_GameScore_4, 
                                   vo->GameScore_4, 
                                   sizeof(m_fivePKModelData->m_GameScore_4));
                            memcpy(m_fivePKModelData->m_GameScore_5, 
                                   vo->GameScore_5, 
                                   sizeof(m_fivePKModelData->m_GameScore_5));

                            m_fivePKModelData->m_BetRatio = vo->BetRatio;

                            memcpy(m_fivePKModelData->m_GameRatio, 
                                   vo->GameRatio, 
                                   sizeof(m_fivePKModelData->m_GameRatio));

                            m_fivePKModelData->m_MaxBet = vo->MaxBet;
                            m_fivePKModelData->m_MinBet = vo->MinBet;
                            m_fivePKModelData->m_PointRatioL = vo->PointRatioL;
                            m_fivePKModelData->m_PointRatioR = vo->PointRatioR;
                            m_fivePKModelData->m_TableNo = vo->TableNo+1;
                            m_fivePKModelData->m_uUserIndex = vo->uUserIndex;
                            //m_fivePKModelData->m_bFirstLoading = true;

                            m_MenInGrp = m_gamegrpModelData->m_MenInGrp;
                            //封包內容5PK用不著先不管
                            getAllUserInfo();
                            //遊戲幕開始
                            SyncCmdSend(SYNCSIGNAL_SCENE_START);
                            /*
                        }
                        else
                        {
                            //遊戲幕開始
                            SyncCmdSend(SYNCSIGNAL_SCENE_START);
                        }*/

                        break;
                    }

                    case SYNCSIGNAL_GAME_BALANCE:   //等待至遊戲結算幕
                    {
                        GCliWSGameBalance_Vo *vo = (GCliWSGameBalance_Vo *)data;

                        m_fivePKModelData->m_uTotalPoints = vo->nUsersAmountPoints;
                        m_fivePKModelData->m_CanCoutinue = vo->nCanContinue;

                        //遊戲幕開始
                        m_fivePKModelData->m_iGameScene = FivePokerModelData::GAMEBALANCE_SCENE;
                        SyncCmdSend(SYNCSIGNAL_SCENE_START);

                        break;
                    }

                    case SYNCSIGNAL_GROUP_BALANCE:  //等待至局結算幕
                    {
                        GCliWSGroupBalance_Vo *vo = (GCliWSGroupBalance_Vo *)data;

                        //遊戲幕開始
                        m_fivePKModelData->m_iGameScene = FivePokerModelData::GROUPBALANCE_SCENE;
                        SyncCmdSend(SYNCSIGNAL_SCENE_START);

                        break;
                    }

                    case SYNCSIGNAL_GROUP_OVER: //等待該局結束
                    {
                        //notify mediator

                        break;
                    }

                    case SYNCSIGNAL_SCENE_START:    //該幕開始
                    {
                        //notify mediator
                        switch(m_fivePKModelData->m_iGameScene)
                        {
                        case FivePokerModelData::GAME_SCENE:    //Gme幕
                        case FivePokerModelData::GAMEBALANCE_SCENE:    //GameBalance幕
                            {
                                sendNotification(GameCliMsg::GCLIMSG_FIRST::Name, (void *)m_userInfoListModelData->UserInfoMap[m_fivePKModelData->m_uUserIndex]);
                                break;
                            }

                        case FivePokerModelData::GROUPBALANCE_SCENE:    //GroupBlance幕
                            {
                                SyncCmdSend(SYNCSIGNAL_GROUP_OVER);

                                //TODO:退回大廳
                                break;
                            }
                        }
                        break;
                    }
                }
            }

            FivePokerModelData *m_fivePKModelData;
        };
    }
}

#endif //_WAITSYNCCOMMAND_HPP_
