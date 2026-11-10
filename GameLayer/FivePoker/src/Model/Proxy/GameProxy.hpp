#ifndef _GAMEPROXY_HPP_
#define _GAMEPROXY_HPP_

#include "Proxy.h"
#include "../ModelData/FivePokerModelData.h"

namespace GameLayer
{
    namespace Game
    {
        using BaseLayer::Patterns::Proxy;

        class GameProxy : public Proxy
        {
        public:
            GameProxy(std::string const &proxy_name = Proxy::NAME,
                      void const *data = NULL)
                : Proxy(proxy_name, data)
            {
                datamap = NULL;

                if(!_data)
                {
                    _data = (void *)new std::map<std::string, IData *>;
                }

                registerModelData("FivePokerModelData", (IData *)new FivePokerModelData());
            }

            virtual ~GameProxy()
            {
                releaseModelData("FivePokerModelData");

                std::map<std::string, IData *> *datamap = (std::map<std::string, IData *> *)_data;
                if(datamap)
                {
                    datamap->clear();
                    delete datamap;
                }
            };

            virtual void onRegister(void)
            {
                datamap = (std::map<std::string, IData *> *)_data;

                Proxy::onRegister();
            };

            virtual void onRemove(void)
            {
                Proxy::onRemove();

                //datamap = (std::map<std::string, IData *> *)_data;
            };

        private:
            std::map<std::string, IData *> *datamap;
        };
    }
}

#endif //_GAMEPROXY_HPP_
