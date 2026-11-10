#include "Facade.h"
#include "../../Common/Common.h"
#include "../Observer/Notification.h"
#include "../../../Core/Controller/Controller.h"
#include "../../../Core/View/View.h"
#include "../../../Core/Model/Model.h"

using namespace BaseLayer::Patterns;

char const *const Facade::MULTITON_MSG =
"Facade instance for this Multiton key already constructed!";

char const *const Facade::DEFAULT_KEY = "BaseLayer";

char const __FACADE_NAME__[] = "Facade";
InstanceMapFacade facade_instance_map;

using namespace BaseLayer::Core;
using namespace BaseLayer::Patterns;

Facade::Facade(std::string const &key /*= Facade::DEFAULT_KEY*/)
    : _controller(NULL)
    , _model(NULL)
    , _view(NULL)
{
    if(facade_instance_map.find(key))
    {
        //TODO:add error log
        //throw std::runtime_error(MULTITON_MSG);
    }

    initializeNotifier(key);

    facade_instance_map.insert(key, this);

    initializeFacade();
}

Facade::~Facade(void)
{
    if(_controller != NULL)
    {
#ifdef _DEBUG_CONSOLE_
        std::cout<<"delete _controller with name:"<<_multiton_key<<std::endl;
#endif
        delete _controller;
        _controller = NULL;
    }

    if(_view != NULL)
    {
#ifdef _DEBUG_CONSOLE_
        std::cout<<"delete _view with name:"<<_multiton_key<<std::endl;
#endif
        delete _view;
        _view = NULL;
    }

    if(_model != NULL)
    {
#ifdef _DEBUG_CONSOLE_
        std::cout<<"delete _model with name:"<<_multiton_key<<std::endl;
#endif
        delete _model;
        _model = NULL;
    }

    facade_instance_map.remove(_multiton_key);

#ifdef _DEBUG_CONSOLE_
    std::cout<<"delete facade with name:"<<_multiton_key<<std::endl;
#endif
}

IFacade *Facade::find(std::string const &key)
{
    return facade_instance_map.find(key);
}

void Facade::insert(std::string const &key, IFacade *facade)
{
    facade_instance_map.insert(key, facade);
}

void Facade::initializeFacade(void)
{
    initializeModel();
    initializeController();
    initializeView();
}

IFacade &Facade::getInstance(std::string const &key /*= Facade::DEFAULT_KEY*/)
{
    IFacade *result = facade_instance_map.find(key);

    if(result  == NULL)
    {
        result = new Facade(key);

        if(result != NULL)
        {
            if(!hasCore(key))
            {
                facade_instance_map.insert(std::make_pair(key, result));
            }
        }
    }

    return *result;
}

inline void Facade::initializeController(void)
{
    if(_controller != NULL)
    {
        return;
    }

    _controller = &Controller::getInstance(_multiton_key);
}

void Facade::initializeModel(void)
{
    if(_model != NULL)
    {
        return;
    }

    _model = &Model::getInstance(_multiton_key);
}

void Facade::initializeView(void)
{
    if (_view != NULL)
    {
        return;
    }

    _view = &View::getInstance(_multiton_key);
}

inline void Facade::registerCommand(std::string const &notification_name,ICommand *command)
{
    if (_controller == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot register command [%s]. Controller is null.", notification_name.c_str());
    }

    _controller->registerCommand(notification_name, command);
}

inline ICommand const &Facade::retrieveCommand(std::string const &notification_name) const
{
    if (_controller == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot retrieve command [%s]. Controller is null.", notification_name.c_str());
    }

    return _controller->retrieveCommand(notification_name);
}

inline ICommand &Facade::retrieveCommand(std::string const &notification_name)
{
    if (_controller == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot retrieve command [%s]. Controller is null.", notification_name.c_str());
    }

    return _controller->retrieveCommand(notification_name);
}

inline ICommand *Facade::removeCommand(std::string const &notification_name)
{
    if (_controller == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot remove command [%s]. Controller is null.", notification_name.c_str());
    }

    return _controller->removeCommand( notification_name );
}

inline bool Facade::hasCommand(std::string const &notification_name) const
{
    if (_controller == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot find command [%s]. Controller is null.", notification_name.c_str());
    }

    return _controller->hasCommand(notification_name);
}

inline void Facade::registerProxy(IProxy *proxy)
{
    if (_model == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot register proxy [%s]. Model is null.", proxy == NULL ? "NULL" : proxy->getProxyName().c_str());
    }

    _model->registerProxy(proxy);
}

inline IProxy const &Facade::retrieveProxy(std::string const &proxy_name) const
{
    if (_model == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot retrieve proxy [%s]. Model is null.", proxy_name.c_str());
    }

    return _model->retrieveProxy(proxy_name);
}

inline IProxy &Facade::retrieveProxy(std::string const &proxy_name)
{
    if (_model == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot retrieve proxy [%s]. Model is null.", proxy_name.c_str());
    }

    return _model->retrieveProxy(proxy_name);
}

inline IProxy *Facade::removeProxy (std::string const &proxy_name)
{
    if (_model == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot remove proxy [%s]. Model is null.", proxy_name.c_str());
    }

    return _model->removeProxy(proxy_name);
}

inline bool Facade::hasProxy(std::string const &proxy_name) const
{
    if (_model == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot find proxy [%s]. Model is null.", proxy_name.c_str());
    }
    return _model->hasProxy(proxy_name);
}

inline void Facade::registerMediator(IMediator *mediator)
{
    if (_view == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot register mediator [%s]. View is null.",
        //mediator == NULL ? "NULL" : mediator->getMediatorName().c_str());
    }

    _view->registerMediator(mediator);
}

inline IMediator const &Facade::retrieveMediator(std::string const &mediator_name) const
{
    if (_view == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot retrieve mediator [%s]. View is null.", mediator_name.c_str());
    }

    return _view->retrieveMediator(mediator_name);
}

inline IMediator &Facade::retrieveMediator(std::string const &mediator_name)
{
    if (_view == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot retrieve mediator [%s]. View is null.", mediator_name.c_str());
    }

    return _view->retrieveMediator(mediator_name);
}

inline IMediator *Facade::removeMediator(std::string const &mediator_name)
{
    if (_view == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot remove mediator [%s]. View is null.", mediator_name.c_str());
    }

    return _view->removeMediator(mediator_name);
}

inline bool Facade::hasMediator(std::string const &mediator_name) const
{
    if (_view == NULL)
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot find mediator [%s]. View is null.", mediator_name.c_str());
    }

    return _view->hasMediator(mediator_name);
}

inline void Facade::sendNotification(std::string const &notification_name,
                                     void const *body /*= NULL*/,
                                     std::string const &type /*= ""*/)
{
    Notification notification(notification_name, body, type);
    notifyObservers(notification);
}

void Facade::initializeNotifier(std::string const &key)
{
    Notifier::initializeNotifier(key);
}

std::string const &Facade::getMultitonKey(void) const
{
    return Notifier::getMultitonKey();
}

inline void Facade::notifyObservers(INotification const &notification)
{
    if (_view != NULL )
    {
        _view->notifyObservers(notification);
    }
}

bool Facade::hasCore(std::string const &key /*= Facade::DEFAULT_KEY*/)
{
#ifdef _DEBUG_CONSOLE_
        std::cout<<"hasCore with name:"<<key<<std::endl;
#endif
    return facade_instance_map.find(key) != NULL;
}

Facade::CoreNames Facade::listCores(void)
{
    return facade_instance_map.getKeyAggregate();
}

void Facade::removeCore(std::string const &key /*= Facade::DEFAULT_KEY*/)
{
    if (facade_instance_map.find(key) == NULL)
    {
        return;
    }

    Model::removeModel( key );
    Controller::removeController(key);
    View::removeView( key );

    facade_instance_map.remove(key);
}

void Facade::broadcastNotification(INotification const &notification)
{
}

void Facade::broadcastNotification(
    std::string const &notification_name,
    void const *body,
    std::string const &type)
{
}
