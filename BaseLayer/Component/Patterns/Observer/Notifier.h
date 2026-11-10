#ifndef _NOTIFIER_H_
#define _NOTIFIER_H_

#include "../../Interfaces/INotifier.h"
#include "../../Interfaces/IFacade.h"

namespace BaseLayer
{
    namespace Patterns
    {
        using Interfaces::INotifier;
        using Interfaces::IFacade;

        class Notifier : public virtual INotifier
        {
        public:
            explicit Notifier(void);

            explicit Notifier(Notifier const &arg);

        public:
            virtual void sendNotification(std::string const &notification_name,
                                          void const *body = NULL,
                                          std::string const &type = "");

            virtual void initializeNotifier(std::string const &key);

            virtual std::string const &getMultitonKey(void) const;

            Notifier &operator=(Notifier const &arg);

            virtual ~Notifier(void);

        protected:
            IFacade &getFacade();

        protected:
            std::string _multiton_key;

            static char const *const MULTITON_MSG;
        };
    }
}

#endif //_NOTIFIER_H_
