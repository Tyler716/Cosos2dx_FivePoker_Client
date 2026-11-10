#ifndef _SERVICECLOSECOMMAND_HPP_
#define _SERVICECLOSECOMMAND_HPP_

#include "Command.h"

#include <string>

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class ServiceCloseCommand : public Command
        {
        public:
            ServiceCloseCommand() : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
                std::string strConnectName = *note.getBody();

                switch(strConnectName)
                {
                    case "Login Server":
                        //TODO:dispatch or do it
                        break;

                    case "Central Server":
                        //TODO:dispatch or do it
                        break;

                }
            }
        };
    }
}

#endif //_SERVICECLOSECOMMAND_HPP_
