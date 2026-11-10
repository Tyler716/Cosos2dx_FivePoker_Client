#include "Notification.h"

#include <typeinfo>

using namespace BaseLayer::Interfaces;
using namespace BaseLayer::Patterns;

Notification::Notification(std::string const &name,
                           void const *body,
                           std::string const &type)
    : INotification()
    , _name(name)
    , _body(body)
    , _type(type)
{
}

Notification::Notification(Notification const &arg)
    : INotification()
    , _name(arg._name)
    , _body(arg._body)
    , _type(arg._type)
{
}

inline std::string const &Notification::getName(void) const
{
    return _name;
}

inline void Notification::setBody(void const *body)
{
    _body = body;
}

inline void const *Notification::getBody(void) const
{
    return _body;
}

inline void Notification::setType(std::string const &type)
{
    _type = type;
}

inline std::string const &Notification::getType(void) const
{
    return _type;
}

inline std::string Notification::toString(void) const
{
    std::string msg;
    toString(msg);
    return msg;
}

void Notification::toString(std::string &arg) const
{
    arg = "Notification Name: ";
    arg += getName();
    arg += "\nBody: ";
    arg += typeid(_body).name();
    arg += "\nType: ";
    arg += _type;
}

Notification &Notification::operator=(Notification const &arg)
{
    _name = arg._name;
    _type = arg._type;
    _body = arg._body;
    return *this;
}

Notification::~Notification(void)
{
}
