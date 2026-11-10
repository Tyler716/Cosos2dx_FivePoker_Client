#ifndef _SOCKETPROXY_H_
#define _SOCKETPROXY_H_

#include "../../Component/Patterns/Proxy/Proxy.h"
#include "../../Component/Net/GameSocketClient.h"
#include "../../Component/Interfaces/INotification.h"
#include "../../Component/Common/IData.h"
#include "ConnectionModelData.h"

//STL include
#include <string>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace BaseLayer
{
    namespace Services
    {
        using Patterns::Proxy;
        using std::string;
        using NET::GameSocketClient;
        using Interfaces::INotification;
        using Common::IData;

        class SocketProxy : public Proxy
        {
        public:
            SocketProxy(std::string const &proxy_name = Proxy::NAME);

            virtual ~SocketProxy();

            virtual void onRegister(void);

            virtual void onRemove(void);

        protected:
            virtual string CommandIDToCommandName(int commandID) = 0;

            GameSocketClient *_client;

        private:
#if defined(_WIN32) || defined(_WIN64)
            static DWORD recvThread(void *arg);
#else
            static void *recvThread(void *arg);
#endif

            //ConnectionModelData *findModel();
#if defined(_WIN32) || defined(_WIN64)
            HANDLE tid;
#else
            pthread_t tid;
#endif

            ConnectionModelData *modelData;
        };
    }
}

#endif //SOCKET
