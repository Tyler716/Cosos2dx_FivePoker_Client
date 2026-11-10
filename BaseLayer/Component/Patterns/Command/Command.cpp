#include "Command.h"

using namespace BaseLayer::Patterns;

Command::Command()
    : INotifier()
    , ICommand()
    , Notifier()
{
}

Command::Command(Command const &arg)
    : INotifier()
    , ICommand()
    , Notifier(arg)
{
}

Command::~Command(void)
{
}

void Command::execute(INotification const &notification)
{
    (void)notification;
}

void Command::initializeNotifier(std::string const &key)
{
    Notifier::initializeNotifier(key);
}

std::string const &Command::getMultitonKey(void) const
{
    return Notifier::getMultitonKey();
}

void Command::sendNotification(std::string const &notification_name,
                      void const *body,
                      std::string const &type)
{
    Notifier::sendNotification(notification_name, body, type);
}

Command &Command::operator=(Command const &arg)
{
    _multiton_key = arg._multiton_key;

    return *this;
}
