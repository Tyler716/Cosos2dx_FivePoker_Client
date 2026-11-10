#ifndef _JOINGROUPRESULTCOMMAND_HPP_
#define _JOINGROUPRESULTCOMMAND_HPP_

#include "Command.h"

#include "../../../Common/CommandList.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class JoinGroupResultCommand : public Command
        {
        public:
            JoinGroupResultCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get JoinGroupResultCommand!"<<std::endl;
#endif
            }
        };
    }
}

#endif //_JOINGROUPRESULTCOMMAND_HPP_
