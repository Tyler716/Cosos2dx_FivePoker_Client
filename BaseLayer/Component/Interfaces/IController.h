#ifndef _ICONTROLLER_H_
#define _ICONTROLLER_H_

#include "INotification.h"
#include "IAggregate.h"
#include "ICommand.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        using Interfaces::ICommand;

        class IController
        {
        public:
            typedef std::auto_ptr<IAggregate<std::string> > NotificationNames;

            virtual void registerCommand(std::string const &notification_name, ICommand *command) = 0;

            virtual void executeCommand(INotification const &notification) = 0;

            virtual ICommand const &retrieveCommand(std::string const &notification_name) const = 0;

            virtual ICommand &retrieveCommand(std::string const &notification_name) = 0;

            virtual ICommand *removeCommand(std::string const &notification_name) = 0;

            virtual bool hasCommand(std::string const &notification_name) const = 0;

            virtual void clearCommand() = 0;

            virtual NotificationNames listNotificationNames(void) const = 0;

            virtual ~IController(void){};
        };
    }
}

#endif //_ICONTROLLER_H_
