#include "Notifier.h"
#include "../Facade/Facade.h"

using namespace BaseLayer::Patterns;
using namespace BaseLayer::Interfaces;

char const *const Notifier::MULTITON_MSG =
        "Multiton key for this Notifier not yet initialized!";

Notifier::Notifier(void)
    : INotifier()
{
}

Notifier::Notifier(Notifier const &arg)
    : INotifier()
    , _multiton_key(arg._multiton_key)
{
}

void Notifier::sendNotification(std::string const &notification_name,
                                       void const *body,
                                       std::string const &type)
{
    getFacade().sendNotification(notification_name, body, type);
}

void Notifier::initializeNotifier(std::string const &key)
{
    _multiton_key = key;
}

std::string const &Notifier::getMultitonKey(void) const
{
    return _multiton_key;
}

IFacade &Notifier::getFacade(void)
{
    if (_multiton_key == "" )
    {
        //TODO:add error log
        //throw std::runtime_error(MULTITON_MSG);
        _multiton_key = Facade::DEFAULT_KEY;
    }
    return Facade::getInstance(_multiton_key);
}

Notifier &Notifier::operator=(Notifier const &arg)
{
    _multiton_key = arg._multiton_key;
    return *this;
}

Notifier::~Notifier(void)
{
}
