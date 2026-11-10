#include "Controller.h"
#include "../../Component/Common/Common.h"
#include "../../Component/Patterns/Observer/Observer.h"
#include "../../Component/Patterns/Iterator/Iterator.h"

#include "../View/View.h"

using BaseLayer::Core::Controller;

using namespace BaseLayer::Interfaces;
using namespace BaseLayer::Patterns;

char const *const Controller::MULTITON_MSG =
"Controller instance for this Multiton key already constructed!";

char const *const Controller::DEFAULT_KEY = "BaseLayer";

char const __CONTROLLER_NAME__[] = "Controller";

Controller::Controller(std::string const &key /*= Controller::DEFAULT_KEY*/)
    : _multiton_key(key)
    , _view(NULL)
{
    if (controller_instance_map.find(_multiton_key))
    {
        //TODO:add error log
        //throw std::runtime_error(MULTITON_MSG);
    }

    controller_instance_map.insert(_multiton_key, this);
    initializeController();
}

inline Controller::CommandMap::key_type const &Controller::IteratorConverter::operator()(
    Controller::CommandMap::const_iterator const &iterator) const
{
    return iterator->first;
}

inline Controller::CommandMap::const_iterator
Controller::IteratorRange::begin(Controller::CommandMap const *const &iterator) const
{
    return iterator->begin();
}

inline Controller::CommandMap::const_iterator
Controller::IteratorRange::end(Controller::CommandMap const *const &iterator) const
{
    return iterator->end();
}

IController *Controller::find(std::string const &key)
{
    return controller_instance_map.find(key);
}

void Controller::insert(std::string const &key, IController *controller)
{
    controller_instance_map.insert(key, controller);
}

inline void Controller::initializeController(void)
{
    if(_view != NULL)
        return;

    _view  = &View::getInstance(_multiton_key);
}

IController &Controller::getInstance(std::string const &key /*= Controller::DEFAULT_KEY*/)
{
    IController *result = controller_instance_map.find(key);

    if(result == NULL)
    {
        result = new Controller(key);
        controller_instance_map.insert(key, result);
    }

    return *result;
}

void Controller::executeCommand(INotification const &notification)
{
    CommandMap::iterator result = _command_map.end();

    do
    {
        result = _command_map.find(notification.getName());

        if(result == _command_map.end())
            return;
    }while (false);

    result->second->execute(notification);
}

void Controller::registerCommand(std::string const &notification_name, ICommand *command)
{
    if(_command_map.find(notification_name) == _command_map.end() )
    {
        if(_view == NULL)
        {
            //TODO:add error log
            //throwException<std::runtime_error>("Cannot register command [%s]. View is null.", notification_name.c_str());
        }

        IObserver *observer = createObserver(&Controller::executeCommand, this);

#if defined(_DEBUG_CONSOLE_)
        std::cout<<"registerCommand Name:"<<notification_name<<", this:"<<this<<", observer:"<<observer<<std::endl;
#endif

        observer_instance_map.insert(observer);
        _view->registerObserver(notification_name, observer);

        command->initializeNotifier(_multiton_key);
        _command_map[notification_name] =  command;
    }
}

inline bool Controller::hasCommand(std::string const &notification_name) const
{
    return _command_map.find(notification_name) != _command_map.end();
}

ICommand *Controller::removeCommand(std::string const &notification_name)
{
    CommandMap::value_type::second_type command = NULL;

    do
    {
        // Retrieve the named mediator
        CommandMap::iterator result = _command_map.find(notification_name);

        if(result == _command_map.end() )
            break;

        // get mediator
        command = result->second;
        // remove the mediator from the map
        _command_map.erase(result);
    }while (false);

    if (command != NULL)
    {
        if(_view == NULL)
        {
            //TODO:add error log
            //throwException<std::runtime_error>("Cannot remove command with notification name: [%s]. View is null.", notification_name.c_str());
        }

        // remove the observer
        _view->removeObserver(notification_name, (IObserver*)this);
    }
    return command;
}

void Controller::clearCommand()
{
    std::string notification_name;
    CommandMap::value_type::second_type command = NULL;

#if defined(_DEBUG_CONSOLE_)
    std::cout<<"command size:"<<_command_map.size()<<std::endl;

    int iCommandSize = 0;
#endif
    CommandMap::iterator result = _command_map.begin();

    for(; result != _command_map.end(); ++result)
    {
#if defined(_DEBUG_CONSOLE_)
        iCommandSize++;
#endif
        // get mediator
        notification_name = result->first;
        command = result->second;
        // remove the mediator from the map

        if(command != NULL)
        {
            if(_view == NULL)
            {
                //TODO:add error log
                //throwException<std::runtime_error>("Cannot remove command with notification name: [%s]. View is null.", notification_name.c_str());
            }

            // remove the observer
            _view->removeObserver(notification_name, NULL);

            delete command;
        }
    }

#if defined(_DEBUG_CONSOLE_)
    if(iCommandSize != 0)
    {
        std::cout<<"find command size:"<<iCommandSize<<std::endl;
    }
#endif

    _command_map.clear();
}

ICommand const &Controller::retrieveCommand(std::string const &notification_name) const
{
    CommandMap::const_iterator result = _command_map.find(notification_name);

    if(result == _command_map.end())
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot find any command with notification name: [%s].", notification_name.c_str());
    }

    return *result->second;
}

inline ICommand &Controller::retrieveCommand(std::string const &notification)
{
    return const_cast<ICommand&>(static_cast<Controller const&>(*this).retrieveCommand(notification));
}

void Controller::removeController(std::string const &key /*= Controller::DEFAULT_KEY*/)
{
    controller_instance_map.remove(key);
}

Controller::NotificationNames Controller::listNotificationNames(void) const
{
    typedef StdContainerAggregate<CommandMap const*,
                                  CommandMap::key_type,
                                  CommandMap::const_iterator,
                                  IteratorConverter,
                                  IteratorRange> result_t;

    return std::auto_ptr<IAggregate<CommandMap::key_type> >(new result_t(&_command_map));
}

Controller::~Controller(void)
{
#ifdef _DEBUG_CONSOLE_
    std::cout<<"destruct Controller with name:"<<_multiton_key<<std::endl;
#endif
    clearCommand();
    removeController(_multiton_key);
    _command_map.clear();
}
