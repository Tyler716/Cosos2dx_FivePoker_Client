#ifndef _INSTANCEMAP_H_
#define _INSTANCEMAP_H_

#include "../Patterns/Iterator/Iterator.h"

// C library include
#include <stdio.h>
// STL include
#include <map>
#include <string>
#include <iostream>

#endif //_INSTANCEMAP_H_

namespace BaseLayer
{
    namespace Common
    {
        using namespace Patterns;

        template<typename _Type, char const _Name[] = "BaseLayer">
        class InstanceMap
        {
        private:
            typedef std::map<std::string, _Type *> StringTypeMap;

            StringTypeMap *_instance;

        private:
            class KeyConverter
                : public std::unary_function<typename StringTypeMap::const_iterator, typename StringTypeMap::key_type>
            {
            public:
                inline typename StringTypeMap::key_type const& operator()(
                    typename StringTypeMap::const_iterator const& iterator) const
                {
                    return iterator->first;
                }
            };

            class KeyRange
            {
            public:
                inline typename StringTypeMap::const_iterator begin(StringTypeMap* const& iterator) const
                {
                    return iterator->begin();
                }

                inline typename StringTypeMap::const_iterator end(StringTypeMap* const& iterator) const
                {
                    return iterator->end();
                }
            };

        public:
            InstanceMap(void)
                : _instance(NULL)
            {
            }

            void createCache(void)
            {
                if (_instance != NULL) return;

#if defined(_DEBUG_CONSOLE_)
                std::cout << "Create instance map of " << _Name << std::endl;
#endif
                _instance = new StringTypeMap;
            }

            void insert(std::pair<std::string, _Type *> const& item)
            {
                createCache();

                _instance->insert(item);
            }

            void insert(_Type *const &value)
            {
                createCache();

                char buff[20] = { 0 };

                sprintf(buff, "%lx", (unsigned long)value);

                std::pair<std::string, _Type *> item(buff, value);
                _instance->insert(item);
            }

            void insert(std::string const &key, _Type *const &value)
            {
                createCache();
                std::pair<std::string, _Type *> item(key, value);

                _instance->insert(item);
            }

            _Type *remove(std::string const &key)
            {
                _Type *result = NULL;

                if (_instance == NULL) return result;

                typename StringTypeMap::iterator iter = _instance->find(key);

                if (iter != _instance->end())
                {
                    result = iter->second;
                    _instance->erase(iter);
                }

                return result;
            }

            _Type *remove(_Type *const &value)
            {
                char buff[20] = { 0 };
                sprintf(buff, "%lx", (unsigned long)value);

                return remove(buff);
            }

            _Type *find(std::string const &key) const
            {
                _Type *result = NULL;

                if (_instance == NULL) return result;

                typename StringTypeMap::const_iterator iter= _instance->find(key);

                if (iter != _instance->end())
                {
                    result = iter->second;
                }

                return result;
            }

            _Type *find(_Type *const &value) const
            {
                char buff[20] = { 0 };
                sprintf(buff, "%lx", (unsigned long)value);

                return find(buff);
            }

            template<typename _InsertIterator>
            void getNames(_InsertIterator inserter) const
            {
                if (_instance == NULL) return;

                typename StringTypeMap::const_iterator iter = _instance->begin();
                for(; iter != _instance->end(); ++iter, ++inserter)
                {
                    inserter = iter->first;
                }
            }

            template<typename _Predicate>
            void forEachItem(_Predicate predicate) const
            {
                if (_instance == NULL) return;

                if (_instance == NULL) return;

                typename StringTypeMap::const_iterator iter = _instance->begin();
                for(; iter != _instance->end(); ++iter)
                {
                    predicate(*iter);
                }
            }

            template<typename _Predicate>
            void forEachItem(_Predicate predicate)
            {
                static_cast<InstanceMap const&>(*this).forEachItem(predicate);
            }

            template<typename _Predicate>
            void forEachKey(_Predicate predicate) const
            {
                if (_instance == NULL) return;

                typename StringTypeMap::const_iterator iter = _instance->begin();
                for(; iter != _instance->end(); ++iter)
                {
                    predicate(iter->first);
                }
            }

            template<typename _Predicate>
            void forEachKey(_Predicate predicate)
            {
                static_cast<InstanceMap const&>(*this).forEachKey(predicate);
            }

            template<typename _Predicate>
            void forEachValue(_Predicate predicate) const
            {
                if (_instance == NULL) return;

                typename StringTypeMap::const_iterator iter = _instance->begin();
                for(; iter != _instance->end(); ++iter)
                {
                    predicate(*(iter->second));
                }
            }

            template<typename _Predicate>
            void forEachValue(_Predicate predicate)
            {
                static_cast<InstanceMap const&>(*this).forEachValue(predicate);
            }

            std::auto_ptr<Interfaces::IAggregate<typename StringTypeMap::key_type> > getKeyAggregate()
            {
                createCache();

                typedef StdContainerAggregate<StringTypeMap*,
                                              typename StringTypeMap::key_type,
                                              typename StringTypeMap::const_iterator,
                                              KeyConverter,
                                              KeyRange> result_t;

                return std::auto_ptr<Interfaces::IAggregate<typename StringTypeMap::key_type> >(new result_t(_instance));
            }

            void cleanCache(void)
            {
                if (_instance == NULL) return;

#if defined(_DEBUG_CONSOLE_)
                std::cout << "Clean instance map of " << _Name <<" - Number object: " << _instance->size() << "\r\n";
#endif

                StringTypeMap * instance = _instance;
                _instance = NULL;
                typename StringTypeMap::iterator result = instance->begin();

                /*
                for(; result != instance->end(); ++result)
                {
                    delete result->second;
                }*/

                instance->clear();
                delete instance;
            }

            public:
                ~InstanceMap(void)
                {
                    cleanCache();
                }
        };
    }
}
