#ifndef _NG_MEDIATOR_HPP_
#define _NG_MEDIATOR_HPP_

#include "Mediator.h"
#include "ConnectionModelData.h"
#include "CommandList.h"
#include "GlobalVar.h"
#include "../../Model/vo/GameMsg.h"

#include "cocos2d.h"
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

USING_NS_CC;

namespace GameLayer
{
    namespace Game
    {
        class NGMediator : public Mediator
        {
        public:
            //copy constructor
            explicit NGMediator(NGMediator const &arg);

        public:
            NGMediator(std::string const &mediator_name = Mediator::NAME,
                       void const *view_component = NULL)
                : Mediator(mediator_name, view_component)
            {
                m_pGameProxy = NULL;
                m_pFivePKModelData = NULL;
            }

            virtual Mediator::NotificationNames listNotificationInterests(void) const
            {
                typedef std::list<std::string> list_t;
                typedef StdContainerAggregate<list_t> result_t;

                result_t* result = new result_t;

                result->get().push_back(GameCliMsg::GCLIMSG_FIRST::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_BET_FAIL::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_DEAL::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_DRAW::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_GAME_OVER::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_SET_MULTIPLE_RESULT::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_EXIT_GAME::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_RECORD_INFO::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_TALK::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_CAN_KEEP_TABLE::Name);

                return NotificationNames(result);
            }

            virtual void handleNotification(INotification const &notification)
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get NGMediator Notification:"<<notification.getName()<<std::endl;
#endif
                int iCmdID = GameCliMsg_map[notification.getName()];

                //Command會由GameCence取出並分派
                switch(iCmdID)
                {
                    case GameCliMsg::GCLIMSG_BET_FAIL::id:  //下注失敗
                    case GameCliMsg::GCLIMSG_DEAL::id:  //第一次開牌資料
                    case GameCliMsg::GCLIMSG_GAME_OVER::id:  //遊戲結束
                    case GameCliMsg::GCLIMSG_TALK::id:  //通知遊戲廣播訊息
                    case GameCliMsg::GCLIMSG_CAN_KEEP_TABLE::id:  //是否可以保留該桌
                    {
                        Message *pMsg = new Message(iCmdID, (void *)notification.getBody());
                        g_GameMsg.Add(pMsg);

                        break;
                    }

                    case GameCliMsg::GCLIMSG_FIRST::id:  //遊戲開始
                    {
                        //TODO:更新Data FivePokerModelData/GCliUserInfo_Vo
                        GCliUserInfo_Vo *vo = (GCliUserInfo_Vo *)notification.getBody();

                        if(!m_pGameProxy)
                        {
                            if(getFacade().hasProxy("GameProxy"))
                            {
                                m_pGameProxy = &getFacade().retrieveProxy("GameProxy");

                                std::map<std::string, IData *> *Game_datamap = (std::map<std::string, IData *> *)m_pGameProxy->getData();

                                m_pFivePKModelData = (FivePokerModelData *)Game_datamap->find("FivePokerModelData")->second;
                            }
                        }
                        
                        if(m_pFivePKModelData)
                        {
                            m_pFivePKModelData->m_uUserIndex =  vo->uUserIndex;
                            m_pFivePKModelData->m_uTotalPoints = vo->uTotalPoints;

                            // create the application instance
                            CCDirector *pDirector = CCDirector::sharedDirector();

                            CCScene *pScene = NULL;
                            static bool bFirst = true;

                            if(_view_component)
                            {
                                pScene = (CCScene *)_view_component;
                            }

                            if(bFirst)
                            {
                                if(pScene)
                                {
                                    bFirst = false;
                                    m_pFivePKModelData->m_PreView = m_pFivePKModelData->m_View;
                                    m_pFivePKModelData->m_View = GameView::VIEW_NG;
                                    ((CCLayerMultiplex*)pScene->getChildByTag(0))->switchTo(GameView::VIEW_NG);
                                    //pDirector->replaceScene(pScene);
                                }
                                else
                                {
                                    //TODO: error log
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Scene doesn't exiet!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                                }
                            }
                            else
                            {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "NGMediator add msg:%i", iCmdID);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                                Message *pMsg = new Message(iCmdID, (void *)notification.getBody());
                                g_GameMsg.Add(pMsg);

                                //TODO:error log
                            }
                        }
                        break;
                    }

                    case GameCliMsg::GCLIMSG_DRAW::id:  //第二次開牌資料
                    {
                        GCliMsgDraw_Vo *vo = (GCliMsgDraw_Vo *)notification.getBody();
                        m_pFivePKModelData->m_CanDoubleGame = vo->CanDoubleGame;
                        Message *pMsg = new Message(iCmdID, (void *)notification.getBody());
                        g_GameMsg.Add(pMsg);

                        break;
                    }

                    case GameCliMsg::GCLIMSG_SET_MULTIPLE_RESULT::id:  //設定倍率的結果
                    {
                        CGGCliSetMultipleResult_Vo *vo = (CGGCliSetMultipleResult_Vo *)notification.getBody();

                        if(_view_component)
                        {
                            if(vo->Succ)
                            {
                                memcpy(m_pFivePKModelData->m_GameScore_1,
                                   vo->GameScore_1,
                                   sizeof(m_pFivePKModelData->m_GameScore_1));
                                memcpy(m_pFivePKModelData->m_GameScore_2, 
                                       vo->GameScore_2,
                                       sizeof(m_pFivePKModelData->m_GameScore_2));
                                memcpy(m_pFivePKModelData->m_GameScore_3, 
                                       vo->GameScore_3,
                                       sizeof(m_pFivePKModelData->m_GameScore_3));
                                memcpy(m_pFivePKModelData->m_GameScore_4, 
                                       vo->GameScore_4,
                                       sizeof(m_pFivePKModelData->m_GameScore_4));
                                memcpy(m_pFivePKModelData->m_GameScore_5, 
                                       vo->GameScore_5,
                                       sizeof(m_pFivePKModelData->m_GameScore_5));

                                m_pFivePKModelData->m_Multiple = vo->MultipleIndex;

                                Message *pMsg = new Message(iCmdID, (void *)notification.getBody());
                                g_GameMsg.Add(pMsg);
                            }
                            
                        }
                        break;
                    }

                    case GameCliMsg::GCLIMSG_EXIT_GAME::id:  //離開遊戲
                    {
                        break;
                    }

                    case GameCliMsg::GCLIMSG_RECORD_INFO::id:  //玩家每局記錄,共50局
                    {
                        break;
                    }

                    default:
                    {
                        //TODO:print error msg
                        break;
                    }
                }
            }

            virtual void onRegister(void)
            {

            }

            virtual void onRemove(void)
            {

            }

            virtual ~NGMediator()
            {

            }

        private:
            IProxy *m_pGameProxy;
            FivePokerModelData *m_pFivePKModelData;
        };
    }
}

#endif //_NG_MEDIATOR_HPP_
