#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "../../Interfaces/ICommand.h"
#include "../Observer/Notifier.h"

// STL include
#include <string>
#include <list>

namespace BaseLayer
{
    namespace Patterns
    {
        using Interfaces::ICommand;
        using Interfaces::INotification;

        class Command
            : public virtual ICommand
            , public Notifier
        {
        public:
            explicit Command(void);

            explicit Command(Command const &arg);

        public:
            virtual void execute(INotification const &notification);

            virtual void initializeNotifier(std::string const &key);

            virtual std::string const &getMultitonKey(void) const;

            virtual void sendNotification(std::string const &notification_name,
                                          void const *body = NULL,
                                          std::string const &type = "");

            Command &operator=(Command const &arg);

            virtual ~Command(void);
        };
    }
}

#endif //_COMMAND_H_
