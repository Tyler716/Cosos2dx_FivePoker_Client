#ifndef _MODEL_H_
#define _MODEL_H_

#include "../../Component/Interfaces/IModel.h"
#include "../../Component/Interfaces/IProxy.h"

// STL include
#include <string>
#include <map>
#include <iostream>

namespace BaseLayer
{
    namespace Core
    {
        using Interfaces::IModel;
        using Interfaces::IProxy;

        class Model : public virtual IModel
        {
        protected:
            typedef std::map<std::string, IProxy*> ProxyMap;

        protected:
            /**
             * Define iterator converter for getting proxy name only.
             */
            struct IteratorConverter
                : public std::unary_function<ProxyMap::const_iterator, ProxyMap::key_type>
            {
                /**
                 * Converting operator.
                 *
                 * @param iterator the iterator of internal container.
                 * @return the name of proxy inside iterator.
                 */
                ProxyMap::key_type const &operator()(ProxyMap::const_iterator const &iterator) const;
            };

            /**
             * Define iterator range of internal container.
             */
            struct IteratorRange
            {
                /**
                 * Get begin point.
                 *
                 * @return the begin of iterator.
                 */
                ProxyMap::const_iterator begin(ProxyMap const *const &iterator) const;

                /**
                 * Get end point.
                 *
                 * @return the end of iterator.
                 */
                ProxyMap::const_iterator end(ProxyMap const *const &iterator) const;
            };

        public:
            static char const *const DEFAULT_KEY;

        protected:
            // Message Constants
            static char const *const MULTITON_MSG;

            // The Multiton Key for this Core
            std::string const _multiton_key;

            // Mapping of proxyNames to IProxy instances
            ProxyMap _proxy_map;

        private:
            /**
              *Copy Constructor
             */
            Model(const Model&);
            Model(const IModel&);
            Model &operator=(const Model&);
            Model &operator=(const IModel&);

        protected:
            static IModel *find(const std::string &key);
            static void insert(const std::string &key, IModel *model);

        public:
            /**
             * Constructor.
             *
             * <P>
             * This <code>IModel</code> implementation is a Multiton,
             * so you should not call the constructor
             * directly, but instead call the static Multiton
             * Factory method <code>Model.getInstance( multitonKey )</code>
             *
             * @throws Error Error if instance for this Multiton key instance has already been constructed
             *
             */
            explicit Model(const std::string &key = Model::DEFAULT_KEY);

        protected:
            /**
             * Constructor.
             *
             * <P>
             * Support call virtual method in constructor of base class (<code>Model</code>).
             *
             * Condition: Derived class (class inherit from <code>Model</code>) must implement method: initializeModel
             *
             * Please use this way:
             * <code>
             *
             * ConcreateModel::ConcreateModel(void)
             * :Model(this)
             * {
             *     // Your code here
             * }
             * </code>
             *
             * Factory method <code>Model::getInstance( key )</code>
             *
             * @throws Error Error if instance for this Multiton key has already been constructed
             *
             */
            template<typename _DerivedType>
            explicit Model(_DerivedType *instance, std::string const &key = Model::DEFAULT_KEY)
                : _multiton_key(key)
            {
                if(find(key))
                {
                    //throw std::runtime_error(MULTITON_MSG);
                }

                insert(_multiton_key, this);
                instance->_DerivedType::initializeModel();
            }

            /**
             * Initialize the <code>Model</code> instance.
             *
             * <P>
             * Called automatically by the constructor, this
             * is your opportunity to initialize the Singleton
             * instance in your subclass without overriding the
             * constructor.</P>
             *
             * @return void
             */
            virtual void initializeModel(void);

        public:
            /**
             * <code>Model</code> Multiton Factory method.
             *
             * @return the instance for this Multiton key
             */
            static IModel &getInstance(std::string const &key = Model::DEFAULT_KEY);

            /**
             * Register an <code>IProxy</code> with the <code>Model</code>.
             *
             * @param proxy an <code>IProxy</code> to be held by the <code>Model</code>.
             */
            virtual void registerProxy(IProxy *proxy);

            /**
             * Retrieve an <code>IProxy</code> from the <code>Model</code>.
             *
             * @param proxy_name
             * @return the <code>IProxy</code> instance previously registered with the given <code>proxyName</code>.
             */
            virtual IProxy const &retrieveProxy(std::string const &proxy_name) const;

            /**
             * Retrieve an <code>IProxy</code> from the <code>Model</code>.
             *
             * @param proxy_name
             * @return the <code>IProxy</code> instance previously registered with the given <code>proxyName</code>.
             */
            virtual IProxy &retrieveProxy(std::string const &proxy_name);

            /**
             * Check if a Proxy is registered
             *
             * @param proxy_name
             * @return whether a Proxy is currently registered with the given <code>proxyName</code>.
             */
            virtual bool hasProxy(std::string const &proxy_name) const;

            /**
             * Remove an <code>IProxy</code> from the <code>Model</code>.
             *
             * @param proxy_name name of the <code>IProxy</code> instance to be removed.
             * @return the <code>IProxy</code> that was removed from the <code>Model</code>
             */
            virtual IProxy *removeProxy(std::string const &proxy_name);

            /**
             * Clear all proxy and delete pointer
             */
            virtual void clearProxy();

            /**
             * List all names of proxy
             *
             * @return the aggregate container of <code>proxy_name</code>.
             */
            virtual ProxyNames listProxyNames(void) const;

            /**
             * Remove an IModel instance
             *
             * @param key of IModel instance to remove
             */
            static void removeModel(std::string const &key = Model::DEFAULT_KEY);

            /**
             * Virtual destructor.
             */
            virtual ~Model(void);
        };
    }
}

#endif //_MODEL_H_
