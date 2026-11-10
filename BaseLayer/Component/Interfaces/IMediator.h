#ifndef _IMEDIATOR_H_
#define _IMEDIATOR_H_

#include "INotifier.h"
#include "IAggregate.h"
#include "INotification.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        class IMediator : public virtual INotifier
        {

        public:
            typedef std::auto_ptr<IAggregate<std::string> > NotificationNames;


        public:
            virtual std::string const& getMediatorName(void) const = 0;

            virtual void const* getViewComponent(void) const = 0;

            virtual void setViewComponent(void const* view_component) = 0;

            virtual NotificationNames listNotificationInterests(void) const = 0;

            virtual void handleNotification(INotification const& notification) = 0;

            virtual void onRegister(void){};

            virtual void onRemove(void) = 0;

            virtual ~IMediator(void){};
        };
    }
}

#endif //_IMEDIATOR_H_
