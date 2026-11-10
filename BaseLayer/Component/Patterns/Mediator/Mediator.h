#ifndef _MEDIATOR_H_
#define _MEDIATOR_H_

#include "../../Interfaces/IMediator.h"
#include "../../Interfaces/INotifier.h"
#include "../Observer/Notifier.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Patterns
    {
        using Interfaces::IMediator;
        using Interfaces::INotifier;
        using Interfaces::INotification;

        class Mediator
            : public virtual IMediator
            , public Notifier
        {
        public:
            //copy constructor
            explicit Mediator(Mediator const &arg);

        public:
            Mediator(std::string const &mediator_name = Mediator::NAME, void const *view_component = NULL);

            virtual std::string const &getMediatorName(void) const;

            virtual void setViewComponent(void const *view_component);

            virtual void const *getViewComponent(void) const;

            virtual Mediator::NotificationNames listNotificationInterests(void) const;

            virtual void handleNotification(INotification const &notification);

            virtual void onRegister(void);

            virtual void onRemove(void);

            virtual void initializeNotifier(std::string const &key);

            virtual std::string const &getMultitonKey(void) const;

            virtual void sendNotification(std::string const &notification_name,
                                          void const *body = NULL,
                                          std::string const &type = "");

            //copy operator
            Mediator &operator=(Mediator const &arg);

            virtual ~Mediator(void);

        protected:
            std::string _mediator_name;

            void const *_view_component;

        public:
            static char const *const NAME;
        };
    }
}

#endif //_MEDIATOR_H_
