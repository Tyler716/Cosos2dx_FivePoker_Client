#include "Model.h"
#include "../../Component/Common/Common.h"
#include "../../Component/Patterns/Iterator/Iterator.h"

using BaseLayer::Core::Model;
using BaseLayer::Interfaces::IModel;
using BaseLayer::Interfaces::IProxy;
using BaseLayer::Patterns::StdContainerAggregate;

char const *const Model::MULTITON_MSG =
"Model instance for this Multiton key already constructed!";

char const *const Model::DEFAULT_KEY = "BaseLayer";

char const __MODEL_NAME__[] = "Model";
char const _Event_NAME_[] = "Event";

Model::Model(std::string const &key /*= Model::DEFAULT_KEY*/)
    : _multiton_key(key)
{
    if (model_instance_map.find(_multiton_key))
    {
        //TODO:add error log
        //throw std::runtime_error(MULTITON_MSG);
    }

    model_instance_map.insert(_multiton_key, this);

    initializeModel();
}

inline Model::ProxyMap::key_type const &Model::IteratorConverter::operator()(
    Model::ProxyMap::const_iterator const& iterator) const
{
    return iterator->first;
}

inline Model::ProxyMap::const_iterator Model::IteratorRange::begin(
    Model::ProxyMap const* const& iterator) const
{
    return iterator->begin();
}

inline Model::ProxyMap::const_iterator Model::IteratorRange::end(
    Model::ProxyMap const* const& iterator) const
{
    return iterator->end();
}

IModel *Model::find(std::string const &key)
{
    return model_instance_map.find(key);
}

void Model::insert(std::string const &key, IModel *model)
{
    model_instance_map.insert(key, model);
}

IModel &Model::getInstance(std::string const &key /*= Model::DEFAULT_KEY*/)
{
    IModel *result = model_instance_map.find(key) ;

    if (result == NULL)
    {
        result = new Model(key);
        model_instance_map.insert(key, result);
    }

    return *result;
}

inline void Model::initializeModel(void)
{
}

void Model::registerProxy(IProxy *proxy)
{
    proxy->initializeNotifier(_multiton_key);

    ProxyMap::iterator result = _proxy_map.find(proxy->getProxyName());
    _proxy_map.insert(std::make_pair(proxy->getProxyName(), (proxy)));

    //TODO:¹ê§@
    proxy->onRegister();
}

IProxy const &Model::retrieveProxy(std::string const &proxy_name) const
{
    ProxyMap::const_iterator result = _proxy_map.find(proxy_name);

    if (result == _proxy_map.end())
    {
        //TODO:add error log
        //throwException<std::runtime_error>("Cannot find any proxy with name: [%s].", proxy_name.c_str());
#ifdef _DEBUG_CONSOLE_
        std::cout<<"proxy:"<<proxy_name<<" can't be found!"<<std::endl;
#endif
    }

    return *result->second;
}

inline IProxy &Model::retrieveProxy(std::string const &proxy_name)
{
    return const_cast<IProxy&>(static_cast<Model const&>(*this).retrieveProxy(proxy_name));
}

inline bool Model::hasProxy(std::string const &proxy_name) const
{
    return _proxy_map.find(proxy_name) != _proxy_map.end();
}

IProxy *Model::removeProxy(std::string const &proxy_name)
{
    ProxyMap::value_type::second_type proxy = NULL;

    do
    {
        // Retrieve the named mediator
        ProxyMap::iterator result = _proxy_map.find(proxy_name);

        if (result == _proxy_map.end())
        {
            break;
        }

        // get mediator
        proxy = result->second;

        // remove the mediator from the map
        _proxy_map.erase(result);
    }while (false);

    if (proxy != NULL)
    {
        proxy->onRemove();
    }

    return proxy;
}

void Model::clearProxy()
{
    ProxyMap::value_type::second_type proxy = NULL;

#if defined(_DEBUG_CONSOLE_)
    std::cout<<"proxy size:"<<_proxy_map.size()<<std::endl;

    int iProxySize = 0;
#endif
    ProxyMap::iterator result = _proxy_map.begin();

    for(; result != _proxy_map.end(); ++result)
    {
#if defined(_DEBUG_CONSOLE_)
        iProxySize++;
#endif
        // get mediator
        proxy = result->second;
        // remove the mediator from the map

        if (proxy != NULL)
        {
            proxy->onRemove();

            delete proxy;
        }
    }

#if defined(_DEBUG_CONSOLE_)
    if(iProxySize != 0)
    {
        std::cout<<"find proxy size:"<<iProxySize<<std::endl;
    }
#endif

    _proxy_map.clear();
}

void Model::removeModel(std::string const &key /*= Model::DEFAULT_KEY*/)
{
    model_instance_map.remove(key);
}

Model::ProxyNames Model::listProxyNames(void) const
{
    typedef StdContainerAggregate<ProxyMap const*,
                                  ProxyMap::key_type,
                                  ProxyMap::const_iterator,
                                  IteratorConverter,
                                  IteratorRange> result_t;

    return std::auto_ptr<IAggregate<ProxyMap::key_type> >(new result_t(&_proxy_map));
}

Model::~Model(void)
{
#ifdef _DEBUG_CONSOLE_
    std::cout<<"destruct Model with name:"<<_multiton_key<<std::endl;
#endif
    removeModel(_multiton_key);
    clearProxy();
    //_proxy_map.clear();
}
