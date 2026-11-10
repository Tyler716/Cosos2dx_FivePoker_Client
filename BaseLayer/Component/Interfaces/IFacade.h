#ifndef _IFACADE_H_
#define _IFACADE_H_

#include "IProxy.h"
#include "ICommand.h"
#include "IMediator.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        class IFacade : public virtual INotifier
        {
        public:
            virtual void registerProxy(IProxy *proxy) = 0;

            virtual IProxy const &retrieveProxy(std::string const &proxy_name) const = 0;

            virtual IProxy &retrieveProxy(std::string const &proxy_name) = 0;

            virtual IProxy *removeProxy(std::string const &proxy_name) = 0;

            virtual bool hasProxy(std::string const &proxy_name) const = 0;

            virtual void registerCommand(std::string const &notification_name, ICommand *command) = 0;

            virtual ICommand const &retrieveCommand(std::string const &notification_name) const = 0;

            virtual ICommand &retrieveCommand(std::string const &notification_name) = 0;

            virtual ICommand *removeCommand(std::string const &notification_name) = 0;

            virtual bool hasCommand(std::string const &notification_name) const = 0;

            virtual void registerMediator(IMediator *mediator) = 0;

            virtual IMediator const &retrieveMediator(std::string const &mediator_name) const = 0;

            virtual IMediator &retrieveMediator(std::string const &mediator_name) = 0;

            virtual IMediator *removeMediator(std::string const &mediator_name) = 0;

            virtual bool hasMediator(std::string const &mediator_name) const = 0;

            virtual void notifyObservers(INotification const &notification) = 0;

            virtual ~IFacade(void){};
        };
    }
}

#endif //_IFACADE_H_
