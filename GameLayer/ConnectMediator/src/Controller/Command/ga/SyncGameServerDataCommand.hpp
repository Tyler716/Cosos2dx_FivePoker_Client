#ifndef _SYNCGAMESERVERDATA_HPP_
#define _SYNCGAMESERVERDATA_HPP_

#include "Command.h"
#include "CommandList.h"
#include "../../../Model/vo/ga/SLMsg_Vo.h"
#include "../../../Model/vo/ga/CLIMSG_GROUP_SETTED_Vo.h"
#include "../../../Model/ModelData/GCliUserInfoListModelData.h"
#include "../../../Model/ModelData/GameGroupBufferModelData.h"
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;

        class SyncGameServerDataCommand : public Command
        {
        public:
            SyncGameServerDataCommand() : Command()
                , _client(NULL)
                , m_userInfoListModelData(NULL)
                , m_MenInGrp(0)
            {
            }

            virtual ~SyncGameServerDataCommand()
            {
            }

            virtual void execute(INotification const &note)
            {
                if(!m_userInfoListModelData || !_client)
                {
                    if(getFacade().hasProxy("GameServerProxy"))
                    {
                        IProxy *proxy = &getFacade().retrieveProxy("GameServerProxy");

                        if(!m_userInfoListModelData)
                        {
                            m_userInfoListModelData =
                                (GCliUserInfoListModelData *)((std::map<std::string, IData *> *)proxy->getData())->find("GCliUserInfoListModelData")->second;

                            m_gamegrpModelData = 
                                (GameGroupBufferModelData *)((std::map<std::string, IData *> *)proxy->getData())->find("GameGroupBufferModelData")->second;
                        }

                        if(!_client)
                        {
                            ConnectionModelData *Connectmodeldata =
                                (ConnectionModelData *)((std::map<std::string, IData *> *)proxy->getData())->find("ConnectionModelData")->second;

                            if(Connectmodeldata)
                            {
                                _client = Connectmodeldata->_client;
                            }
                        }
                    }
                }

                int cmdId = GameServerEvent_map[note.getName()];

                switch(cmdId)
                {
                    case GameClientServiceEvent::CLIMSG_GROUP_SETTED_Event::id:  //2048
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"Get CLIMSG_GROUP_SETTED_Event!"<<std::endl;
#endif
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Get CLIMSG_GROUP_SETTED_Event!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                        //送出SLMSG_WAIT_SYNC
                        CLIMSG_GROUP_SETTED_Vo *vo = (CLIMSG_GROUP_SETTED_Vo *)note.getBody();

                        m_gamegrpModelData->m_MenInGrp = vo->uMenInGrp;
                        m_MenInGrp = m_gamegrpModelData->m_MenInGrp;

                        //送出取userInfo的command
                        if(InitUserInfo())
                        {
                            getAllUserInfo();

                            //送出同步訊號
                            SyncCmdSend(cmdId);
                        }
                        else
                        {
#ifdef _DEBUG_CONSOLE_
                            std::cout<<"InitUserInfo fail!"<<std::endl;
#endif
                        }
                        break;
                    }

                    case GameClientServiceEvent::CLIMSG_GAME_CONTINUE_Event::id:  //2051
                    {
                        GCliGameContinue_Vo *vo = (GCliGameContinue_Vo *)note.getBody();

                        if(vo->BtnResult == 1)
                        {
                            SyncCmdSend(SYNCSIGNAL_GAME);
                        }
                        else
                        {
                            SyncCmdSend(SYNCSIGNAL_GROUP_BALANCE);
                        }
                        break;
                    }

                    case GameClientServiceEvent::CLIMSG_WAIT_SYNC_Event::id:  //2054
                    {
                        GCliWaitSync_Vo *vo = (GCliWaitSync_Vo *)note.getBody();
                        //判斷sync cmdid
                        waitSync(vo->nSyncSignal, (Byte *)note.getBody());
                        break;
                    }
                }
            }

            virtual void waitSync(int SyncId, Byte *data)
            {
            };

        protected:
            bool InitUserInfo()
            {
                if(!m_userInfoListModelData)
                {
                    return false;
                }

                for(int i = 0; i < m_MenInGrp; i++)
                {
                    GCliUserInfo_Vo *vo = new GCliUserInfo_Vo();
                    vo->u32State = GCliUserInfo_Vo::PLAYER_STATUS_NONE;
                    vo->uFlags = 0;

                    m_userInfoListModelData->UserInfoMap.insert(std::make_pair(i, vo));
                }
                return true;
            }

            void getAllUserInfo()
            {
                GSLUserInfo_Vo vo;
                vo.u32Size = 8 + 4 * m_MenInGrp;
                vo.u32UserIndexArraySize = m_MenInGrp;

                //TODO:server的宣告只宣告一個元素, 可是flash操作卻塞入menInGrp的個數, 待確認, serverLogic似乎沒用到此欄位
                //vo.u32UserIndexArray[i] = i;

                SendData(GameClientServiceEvent::SLMSG_USER_INFO_Event::id, (Byte *)&vo, sizeof(vo));
            }

            void SyncCmdSend(int SyncId)
            {
                GSLWaitSync_Vo syncVo;
                syncVo.nSyncSignal = SyncId;
                SendData(GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::id, (Byte *)&syncVo, sizeof(syncVo));
            }

            UInt m_MenInGrp;
            //int m_UserIndex;
            GCliUserInfoListModelData *m_userInfoListModelData;
            GameGroupBufferModelData *m_gamegrpModelData;

        private:
            void SendData(int cmdId, Byte *buf, int iSize)
            {
                if(_client)
                {
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"syncCommand send cmd:"<<cmdId<<std::endl;
#endif
                    _client->SendData(cmdId, buf, iSize);
                }
                else
                {
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"syncCommand _client doesn't exist!"<<std::endl;
#endif
                }
            }

            GameSocketClient *_client;
        };
    }
}

#endif //_SYNCGAMESERVERDATA_HPP_


