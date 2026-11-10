#ifndef _IMODEL_H_
#define _IMODEL_H_

#include "IProxy.h"
#include "IAggregate.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        class IModel
        {
        public:
            typedef std::auto_ptr<IAggregate<std::string> > ProxyNames;

            virtual void registerProxy(IProxy *proxy) = 0;

            virtual IProxy const &retrieveProxy(std::string const &proxy_name) const = 0;

            virtual IProxy &retrieveProxy(std::string const &proxy_name) = 0;

            virtual IProxy *removeProxy(std::string const &proxy_name) = 0;

            virtual bool hasProxy(std::string const &proxy_name) const = 0;

            virtual void clearProxy() = 0;

            virtual ProxyNames listProxyNames(void) const = 0;

            virtual ~IModel(void){};
        };
    }
}

#endif //_IMODEL_H_
