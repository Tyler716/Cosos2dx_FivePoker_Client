#ifndef _INOTIFICATION_H_
#define _INOTIFICATION_H_

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        class INotification
        {
        public:
            virtual std::string const &getName(void) const = 0;

            virtual void setBody(void const *body) = 0;

            virtual void const *getBody(void) const = 0;

            virtual void setType(std::string const &type) = 0;

            virtual std::string const &getType(void) const = 0;

            virtual std::string toString(void) const = 0;

            virtual void toString(std::string &arg) const = 0;

            virtual ~INotification(void){};
        };
    }
}
#endif //_INOTIFICATION_H_
