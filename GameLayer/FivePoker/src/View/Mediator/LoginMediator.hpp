#ifndef _LOGIN_MEDIATOR_HPP_
#define _LOGIN_MEDIATOR_HPP_

#include "Mediator.h"
#include "GlobalVar.h"
#include "../../Model/vo/GameMsg.h"
#include "cocos2d.h"

USING_NS_CC;

namespace GameLayer
{
    namespace Game
    {
        class LoginMediator : public Mediator
        {
        public:
            //copy constructor
            explicit LoginMediator(LoginMediator const &arg);

        public:
            LoginMediator(std::string const &mediator_name = Mediator::NAME,
                       void const *view_component = NULL)
                : Mediator(mediator_name, view_component)
            {
            }

            virtual Mediator::NotificationNames listNotificationInterests(void) const
            {
                typedef std::list<std::string> list_t;
                typedef StdContainerAggregate<list_t> result_t;

                result_t* result = new result_t;

                result->get().push_back(GameDefineMsg::GAMEMSG_LOGIN_FAIL::Name);
                result->get().push_back(GameDefineMsg::GAMEMSG_NETWORK_OFF::Name);
                result->get().push_back(GameDefineMsg::GAMEMSG_CONNECT_LS_OK::Name);
                
                return NotificationNames(result);
            }

            virtual void handleNotification(INotification const &notification)
            {
                
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get DGMediator Notification:"<<notification.getName()<<std::endl;
#endif
                int iCmdID = GameCliMsg_map[notification.getName()];
                
                switch(iCmdID)
                {
                    case GameDefineMsg::GAMEMSG_LOGIN_FAIL::id: //登入失敗
                    case GameDefineMsg::GAMEMSG_NETWORK_OFF::id:    //網路關閉
                    case GameDefineMsg::GAMEMSG_CONNECT_LS_OK::id:  //連線OK
                    {
                        Message *pMsg = new Message(iCmdID, (void *)notification.getBody());
                        g_GameMsg.Add(pMsg);
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

            virtual ~LoginMediator()
            {

            }
        };
    }
}

#endif //_LOGIN_MEDIATOR_HPP_
