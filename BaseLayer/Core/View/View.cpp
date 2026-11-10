#include "View.h"
#include "../../Component/Common/Common.h"
#include "../../Component/Patterns/Iterator/Iterator.h"
#include "../../Component/Patterns/Observer/Observer.h"

using BaseLayer::Core::View;

using namespace BaseLayer::Interfaces;
using namespace BaseLayer::Patterns;

char const *const View::MULTITON_MSG =
"View instance for this Multiton key already constructed!";

char const *const View::DEFAULT_KEY = "BaseLayer";

char const __VIEW_NAME__[] = "View";
char const __OBSERVER_NAME__[] = "Observer";

View::View(std::string const &key /*= View::DEFAULT_KEY*/)
    : _multiton_key(key)
{
    if(view_instance_map.find(_multiton_key))
    {
        //TODO add error log
        //throw std::runtime_error(MULTITON_MSG);
    }

    view_instance_map.insert(_multiton_key, this);

    initializeView();
}

inline View::MediatorMap::key_type const &View::IteratorConverter::operator()(
    View::MediatorMap::const_iterator const &iterator) const
{
    return iterator->first;
}

inline View::MediatorMap::const_iterator View::IteratorRange::begin(
    View::MediatorMap const *const &iterator) const
{
    return iterator->begin();
}

inline View::MediatorMap::const_iterator View::IteratorRange::end(
    View::MediatorMap const *const &iterator) const
{
    return iterator->end();
}

IView *View::find(std::string const &key)
{
    return view_instance_map.find(key);
}

void View::insert(std::string const &key, IView *view)
{
    view_instance_map.insert(key, view);
}

inline void View::initializeView(void)
{
}

IView &View::getInstance(std::string const &key /*= View::DEFAULT_KEY*/)
{
    IView *result = view_instance_map.find(key);

    if(result == NULL)
    {
        result = new View(key);
        view_instance_map.insert(key, result);
    }

    return *result;
}

inline void View::registerObserver(std::string const &notification_name,
                                   IObserver *observer)
{
    ObserverMap::value_type item(notification_name, observer);

    _observer_map.insert(item);
}

void View::notifyObservers(INotification const &notification)
{
    typedef std::list<IObserver*> list_observer_t;

    std::string const &name = notification.getName();

    list_observer_t list;

    do
    {
        // Find observer by name
        ObserverMap::iterator result = _observer_map.find(name);

        // Looping and store observer
        for(; result != _observer_map.end() && result->first == notification.getName(); ++result)
        {
            list.push_back(result->second);
        }
    }while(false);

    list_observer_t::const_iterator iter = list.begin();

    for(; iter != list.end(); ++iter)
    {
        (*iter)->notifyObserver(notification);
    }
}

void View::removeObserver(std::string const &notification_name,
                          void const *notify_context)
{
    ObserverMap::iterator result = _observer_map.find(notification_name);

    for(; result != _observer_map.end(); ++result)
    {
        if(result->first != notification_name)
            continue;

        ObserverMap::value_type::second_type observer = result->second;

        if(notify_context)
        {
            if(!observer->compareNotifyContext(notify_context))
            {
                return;
            }

            _observer_map.erase(result);
            observer_instance_map.remove(observer);

            delete observer;

            return;
        }
    }
}

void View::registerMediator(IMediator *mediator)
{
    do
    {
        // donot allow re-registration (you must to removeMediator fist)
        if(_mediator_map.find(mediator->getMediatorName()) != _mediator_map.end() )
            return;

        mediator->initializeNotifier(_multiton_key);

        // Register the Mediator for retrieval by name
        _mediator_map.insert(std::make_pair(mediator->getMediatorName(), mediator));
    }while(false);

    IMediator::NotificationNames result(mediator->listNotificationInterests());

    IMediator::NotificationNames::element_type::Iterator iter(result->getIterator());

    // Register Mediator as an observer for each notification of interests
    while(iter->moveNext())
    {
        // Register Mediator as Observer for its list of Notification interests
        // Create Observer referencing this mediator's handlNotification method
        IObserver *observer = createObserver(&IMediator::handleNotification, mediator);
        observer_instance_map.insert(observer);
        registerObserver(iter->getCurrent(), observer);
    }

    // alert the mediator that it has been registered
    mediator->onRegister();
}

inline IMediator const &View::retrieveMediator(std::string const &mediator_name) const
{
    MediatorMap::const_iterator result = _mediator_map.find(mediator_name);

    if(result == _mediator_map.end())
    {
        //throwException<std::runtime_error>("Cannot find any mediator with name: [%s].", mediator_name.c_str());
    }

    return *result->second;
}

inline IMediator &View::retrieveMediator(std::string const &mediator_name)
{
    return const_cast<IMediator &>(static_cast<View const &>(*this).retrieveMediator(mediator_name));
}

IMediator *View::removeMediator(std::string const &mediator_name)
{
    MediatorMap::value_type::second_type mediator = NULL;

    do
    {
        // Retrieve the named mediator
        MediatorMap::iterator result = _mediator_map.find(mediator_name);

        if(result == _mediator_map.end())
            break;

        // Get mediator object
        mediator = result->second;
        // remove the mediator from the map
        _mediator_map.erase(result);
    }while(false);

    if(mediator != NULL)
    {
        // for every notification this mediator is interested in...
        IMediator::NotificationNames result(mediator->listNotificationInterests());

        IMediator::NotificationNames::element_type::Iterator iter(result->getIterator());

        while(iter->moveNext())
        {
            // remove the observer linking the mediator to the notification interest
            removeObserver(iter->getCurrent(), (IObserver*)mediator);
        }

        // Notify that it was removed
        mediator->onRemove();
    }
    return mediator;
}

void View::clearMediator()
{
    MediatorMap::value_type::second_type mediator = NULL;

#if defined(_DEBUG_CONSOLE_)
    std::cout<<"medaitor size:"<<_mediator_map.size()<<std::endl;

    int iMediatorSize = 0;
#endif
    MediatorMap::iterator result = _mediator_map.begin();

    for(; result != _mediator_map.end(); ++result)
    {
#if defined(_DEBUG_CONSOLE_)
        iMediatorSize++;
#endif
        // get mediator
        mediator = result->second;
        // remove the mediator from the map

        if(mediator != NULL)
        {
            // for every notification this mediator is interested in...
            IMediator::NotificationNames result(mediator->listNotificationInterests());

            IMediator::NotificationNames::element_type::Iterator iter(result->getIterator());

            while(iter->moveNext())
            {
                // remove the observer linking the mediator to the notification interest
                removeObserver(iter->getCurrent(), (IObserver*)mediator);
            }

            // Notify that it was removed
            mediator->onRemove();

            delete mediator;
        }
    }

#if defined(_DEBUG_CONSOLE_)
    if(iMediatorSize != 0)
    {
        std::cout<<"find mediator size:"<<iMediatorSize<<std::endl;
    }
#endif

    _mediator_map.clear();
}

inline bool View::hasMediator(std::string const &mediator_name) const
{
    return _mediator_map.find(mediator_name) != _mediator_map.end();
}

void View::removeView(std::string const &key /*= View::DEFAULT_KEY*/)
{
    view_instance_map.remove(key);
}

View::MediatorNames View::listMediatorNames(void) const
{
    typedef StdContainerAggregate<MediatorMap const*,
                                  MediatorMap::key_type,
                                  MediatorMap::const_iterator,
                                  IteratorConverter,
                                  IteratorRange> result_t;

    return std::auto_ptr<IAggregate<MediatorMap::key_type> >(new result_t(&_mediator_map));
}

View::~View(void)
{
#ifdef _DEBUG_CONSOLE_
    std::cout<<"destruct View with name:"<<_multiton_key<<std::endl;
#endif
    removeView(_multiton_key);
    ObserverMap::const_iterator iter = _observer_map.begin();
    for(; iter != _observer_map.end(); ++iter)
    {
        ObserverMap::value_type::second_type observer = iter->second;
        ObserverMap::value_type::second_type observer2 =
            observer_instance_map.remove(observer);

        if(observer2 != NULL)
        {
            delete observer2;
        }
    }

    _observer_map.clear();

    clearMediator();
    //_mediator_map.clear();
}

