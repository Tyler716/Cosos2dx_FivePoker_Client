#ifndef _ACCEPTGAMECLIENTCLOSEDCOMMAND_HPP_
#define _ACCEPTGAMECLIENTCLOSEDCOMMAND_HPP_

#include "Command.h"

#include "../../../Common/CommandList.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class AcceptGameClientClosedCommand : public Command
        {
        public:
            AcceptGameClientClosedCommand()
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
        };
    }
}

#endif //_ACCEPTGAMECLIENTCLOSEDCOMMAND_HPP_
