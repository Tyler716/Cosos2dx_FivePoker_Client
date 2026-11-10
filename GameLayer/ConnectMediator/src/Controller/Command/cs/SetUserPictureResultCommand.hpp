#ifndef _SETUSERPICTURERESULTCOMMAND_HPP_
#define _SETUSERPICTURERESULTCOMMAND_HPP_

#include "Command.h"

#include "../../../Common/CommandList.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class SetUserPictureResultCommand : public Command
        {
        public:
            SetUserPictureResultCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {

            }

            virtual void initializeNotifier(std::string const &key)
            {
                Command::initializeNotifier(key);
            }

            virtual std::string const &getMultitonKey(void) const
            {
                return Command::getMultitonKey();
            }

            virtual void sendNotification(std::string const &notification_name,
                                  void const *body,
                                  std::string const &type)
            {
                Command::sendNotification(notification_name, body, type);
            }
        };
    }
}

#endif //_SETUSERPICTURERESULTCOMMAND_HPP_
