#ifndef _MODELTESTPROXY_HPP_
#define _MODELTESTPROXY_HPP_

#include "../../Component/Patterns/Proxy/Proxy.h"

namespace data
{
    using BaseLayer::Patterns::Proxy;

    struct ModelTestProxy: public Proxy
    {
        static char const* const NAME;
        static char const* const ON_REGISTER_CALLED;
        static char const* const ON_REMOVE_CALLED;

        ModelTestProxy() :Proxy(NAME, "")
        { }

        virtual void onRegister(void)
        {
            setData(ON_REGISTER_CALLED);
        }

        virtual void onRemove(void)
        {
            setData(ON_REMOVE_CALLED);
        }
    };
}

#endif //_MODELTESTPROXY_HPP_
