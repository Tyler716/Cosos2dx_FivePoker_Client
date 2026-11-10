#ifndef _IVIEW_H_
#define _IVIEW_H_

#include "IAggregate.h"
#include "IObserver.h"
#include "IMediator.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        using Interfaces::IMediator;

        class IView
        {
        public:
            typedef std::auto_ptr<IAggregate<std::string> > MediatorNames;

            virtual void registerObserver(std::string const &notification_name, IObserver *observer) = 0;

            virtual void removeObserver(std::string const &notification_name, void const *notify_context) = 0;

            virtual void notifyObservers(INotification const &notification) = 0;

            virtual void registerMediator(IMediator *mediator) = 0;

            virtual IMediator const &retrieveMediator(std::string const &mediator_name) const = 0;

            virtual IMediator &retrieveMediator(std::string const &mediator_name) = 0;

            virtual IMediator *removeMediator(std::string const &mediator_name) = 0;

            virtual bool hasMediator(std::string const &mediator_name) const = 0;

            virtual void clearMediator() = 0;

            virtual MediatorNames listMediatorNames(void) const = 0;

            virtual ~IView(void){};
        };
    }
}

#endif //_IVIEW_H_
