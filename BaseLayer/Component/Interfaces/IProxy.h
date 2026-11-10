#ifndef _IPROXY_H_
#define _IPROXY_H_

#include "INotifier.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        class IProxy : public virtual INotifier
        {
        public:
            virtual std::string const &getProxyName() const = 0;

            virtual void setData(void const *data) = 0;

            virtual void const *getData(void) const = 0;

            virtual void onRegister(void) = 0;

            virtual void onRemove(void) = 0;

            virtual ~IProxy(void){};
        };
    }
}

#endif //_IPROXY_H_
