#ifndef _NOTIFICATION_H_
#define _NOTIFICATION_H_

#include "../../Interfaces/INotification.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Patterns
    {
        using Interfaces::INotification;

        class Notification : public virtual INotification
        {
        public:
            //Copy Contructor
            explicit Notification(Notification const &arg);

            //constructor
            Notification(std::string const &name, void const *body = NULL, std::string const &type = "");

            virtual std::string const &getName(void) const;

            virtual void setBody(void const *body);

            virtual void const *getBody(void) const;

            virtual void setType(std::string const &type);

            virtual std::string const &getType(void) const;

            virtual std::string toString(void) const;

            virtual void toString(std::string &arg) const;

            //Copy Operator
            Notification& operator=(Notification const& arg);

            virtual ~Notification(void);

        private:
            std::string _name;

            void const *_body;

            std::string _type;
        };
    }
}

#endif //_NOTIFICATION_H_
