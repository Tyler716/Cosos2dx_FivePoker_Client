#ifndef _CENTRALSERVERTICKENABLEDCOMMAND_HPP_
#define _CENTRALSERVERTICKENABLEDCOMMAND_HPP_

#include "Command.h"
#include "ConnectionModelData.h"
#include "../../../Model/vo/cs/CS_LB_BOOT_TICK_R_Vo.h"
#include "../../../Common/CommandList.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class CentralServerTickEnabledCommand : public Command
        {
        public:
            CentralServerTickEnabledCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"get CentralServerTickEnabledCommand!"<<std::endl;
#endif
            }
        };
    }
}

#endif //_CENTRALSERVERTICKENABLEDCOMMAND_HPP_
