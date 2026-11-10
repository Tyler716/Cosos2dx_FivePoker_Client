#ifndef _PROXY_H_
#define _PROXY_H_

#include "../../Interfaces/IProxy.h"
#include "../../Common/IData.h"
#include "../Observer/Notifier.h"

// STL include
#include <string>
#include <iostream>

namespace BaseLayer
{
    namespace Patterns
    {
        using Interfaces::IProxy;
        using Interfaces::INotifier;
        using Interfaces::INotification;
        using Common::IData;

        class Proxy
            : public virtual IProxy
            , public Notifier
        {
        public:
            //copy constructor
            explicit Proxy(Proxy const &arg);

        public:
            /**
             * Constructor.
             */
            explicit Proxy(std::string const &proxy_name = Proxy::NAME,
                                       void const *data = NULL);

            virtual std::string const &getProxyName(void) const;

            virtual void setData(void const *data);

            virtual void const *getData(void) const;

            virtual void onRegister(void);

            virtual void onRemove(void);

            virtual void initializeNotifier(std::string const &key);

            virtual std::string const &getMultitonKey(void) const;

            virtual void sendNotification(std::string const &notification_name,
                                          void const *body = NULL,
                                          std::string const &type = "");

            Proxy &operator=(Proxy const &arg);

            virtual ~Proxy(void);

        protected:
            void registerModelData(std::string const &strName, IData *vo);
            void releaseModelData(std::string const &strName);

        protected:
           // the proxy name
           std::string _proxy_name;

           // the data object
           void const *_data;

        public:
            static char const *const NAME;
        };
    }
}

#endif  //_PROXY_H_
