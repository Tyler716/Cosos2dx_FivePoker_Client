#include "Proxy.h"
#include <iostream>
#include <map>

using namespace BaseLayer::Patterns;

char const *const Proxy::NAME = "Proxy";

Proxy::Proxy(std::string const &proxy_name /*= Proxy::NAME*/, void const *data /*= NULL*/)
    : INotifier()
    , IProxy()
    , Notifier()
    , _proxy_name(proxy_name)
    , _data(data)
{
    if(data == NULL)
    {
        std::cout<<"Proxy data == NULL"<<std::endl;
    }
}

Proxy::Proxy(Proxy const &arg)
    : INotifier()
    , IProxy()
    , Notifier(arg)
    , _proxy_name(arg._proxy_name)
    , _data(arg._data)
{
}

Proxy::~Proxy(void)
{
}

inline std::string const &Proxy::getProxyName(void) const
{
    return _proxy_name;
}

inline void Proxy::setData(void const *data)
{
    _data = data;
}

inline void const *Proxy::getData(void) const
{
    return _data;
}

inline void Proxy::onRegister(void)
{
}

inline void Proxy::onRemove(void)
{
}

void Proxy::initializeNotifier(std::string const &key)
{
    Notifier::initializeNotifier(key);
}

std::string const &Proxy::getMultitonKey(void) const
{
    return Notifier::getMultitonKey();
}

void Proxy::sendNotification(std::string const &notification_name,
                      void const *body /*= NULL*/,
                      std::string const &type /*= ""*/)
{
    Notifier::sendNotification(notification_name, body, type);
}

Proxy &Proxy::operator=(Proxy const &arg)
{
    _proxy_name = arg._proxy_name;
    _data = arg._data;
    return *this;
}

void Proxy::registerModelData(std::string const &strName, IData *vo)
{
    std::map<std::string, IData *> *datamap = (std::map<std::string, IData *>*)_data;

    datamap->insert(std::make_pair(strName, vo));
}

void Proxy::releaseModelData(std::string const &strName)
{
    if(_data)
    {
        //移除map中所有註冊的modelData
        std::map<std::string, IData *> *datamap = (std::map<std::string, IData *> *)_data;
        std::map<std::string, IData *>::iterator iter = datamap->begin();

        for(; iter != datamap->end(); ++iter)
        {
            if(iter->first == strName)
            {
                delete ((IData *)iter->second);
            }
        }
    }
}
