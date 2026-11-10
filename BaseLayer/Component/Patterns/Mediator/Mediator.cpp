#include "Mediator.h"
#include "../Iterator/Iterator.h"

#include <list>

using namespace BaseLayer::Patterns;
using namespace BaseLayer::Interfaces;

char const *const Mediator::NAME = "Mediator";

Mediator::Mediator(Mediator const &arg)
    : INotifier()
    , IMediator()
    , Notifier(arg)
    , _mediator_name(arg._mediator_name)
    , _view_component(arg._view_component)
{
}

Mediator::Mediator(std::string const &mediator_name /* = Mediator::NAME*/,
                   void const *view_component /* = NULL*/)
    : INotifier()
    , IMediator()
    , Notifier()
    , _mediator_name(mediator_name)
    , _view_component(view_component)
{
}


Mediator::~Mediator(void)
{
}

inline std::string const &Mediator::getMediatorName(void) const
{
    return _mediator_name;
}

inline void Mediator::setViewComponent(void const *view_component)
{
    _view_component = view_component;
}

inline void const *Mediator::getViewComponent(void) const
{
    return _view_component;
}

inline Mediator::NotificationNames Mediator::listNotificationInterests(void) const
{
    typedef std::list<std::string> list_t;
    typedef StdContainerAggregate<list_t> result_t;

    //std::auto_ptr<IAggregate<std::string> > NotificationNames;

    return NotificationNames(new result_t);
}

inline void Mediator::handleNotification(INotification const &notification)
{
    (void)notification;
}

inline void Mediator::onRegister(void)
{
}

inline void Mediator::onRemove(void)
{
}

void Mediator::initializeNotifier(std::string const &key)
{
    Notifier::initializeNotifier(key);
}

std::string const &Mediator::getMultitonKey(void) const
{
    return Notifier::getMultitonKey();
}

void Mediator::sendNotification(std::string const &notification_name,
                      void const *body,
                      std::string const &type)
{
    Notifier::sendNotification(notification_name, body, type);
}

Mediator &Mediator::operator=(Mediator const &arg)
{
    _mediator_name = arg._mediator_name;
    _view_component = arg._view_component;

    return *this;
}
