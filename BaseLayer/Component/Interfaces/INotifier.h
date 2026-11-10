#ifndef _INOTIFIER_H_
#define _INOTIFIER_H_

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        class INotifier
        {
        public:
            virtual void sendNotification(std::string const &notification_name,
                                          void const *body = NULL,
                                          std::string const &type = "") = 0;

            virtual void initializeNotifier(std::string const &key) = 0;

            virtual std::string const &getMultitonKey(void) const = 0;

            virtual ~INotifier(void){};
        };
    }
}

#endif //_INOTIFIER_H_
