#ifndef _DG_MEDIATOR_HPP_
#define _DG_MEDIATOR_HPP_

#include "Mediator.h"
#include "GlobalVar.h"
#include "../../Model/vo/GameMsg.h"
#include "cocos2d.h"

USING_NS_CC;

namespace GameLayer
{
    namespace Game
    {
        class DGMediator : public Mediator
        {
        public:
            //copy constructor
            explicit DGMediator(DGMediator const &arg);

        public:
            DGMediator(std::string const &mediator_name = Mediator::NAME,
                       void const *view_component = NULL)
                : Mediator(mediator_name, view_component)
            {
            }

            virtual Mediator::NotificationNames listNotificationInterests(void) const
            {
                typedef std::list<std::string> list_t;
                typedef StdContainerAggregate<list_t> result_t;

                result_t* result = new result_t;

                result->get().push_back(GameCliMsg::GCLIMSG_DOUBLE_GAME_DATA::Name);
                result->get().push_back(GameCliMsg::GCLIMSG_DOUBLE_GAME_RESULT::Name);

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
                    case GameCliMsg::GCLIMSG_DOUBLE_GAME_DATA::id:  //¤ñ­¿
                    case GameCliMsg::GCLIMSG_DOUBLE_GAME_RESULT::id:
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

            virtual ~DGMediator()
            {

            }
        };
    }
}

#endif //_DG_MEDIATOR_HPP_
