#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include "../../Interfaces/IAggregate.h"
#include "../../Interfaces/IIterator.h"

// STL include
#include <map>
#include <string>
#include <iostream>

namespace BaseLayer
{
    namespace Patterns
    {
        using Interfaces::IAggregate;
        using Interfaces::IIterator;

        template<typename _Type, typename _IteratorType>
        class DefaultIteratorConverter : public std::unary_function<_IteratorType, _Type>
        {
        public:
            inline _Type const &operator()(_IteratorType const &iterator) const
            {
                return *(const _Type*)(&(*iterator));
            }
        };

        template<typename _Type,
                 typename _IteratorType = _Type,
                 typename _ConverterType = DefaultIteratorConverter<_Type, _IteratorType> >
        class Iterator : public virtual IIterator<_Type>
        {
        protected:
            mutable _IteratorType *_begin;
            mutable _IteratorType *_end;
            mutable _IteratorType *_current;
            mutable _ConverterType _converter;

        public:
            //default constructor
            explicit Iterator(void)
                : _begin(NULL)
                , _end(NULL)
                , _current(NULL)
            {
            }

            explicit Iterator(_IteratorType const &begin, _IteratorType const &end)
                : _begin(new _IteratorType(begin))
                , _end(new _IteratorType(end))
                , _current(NULL)
            {
            }

            /**
             * Copy Constructor
             */
            explicit Iterator(Iterator const &arg)
                : _begin(new _IteratorType(*(arg._begin)))
                , _end(new _IteratorType(*(arg._end)))
                , _current(new _IteratorType(*(arg._current)))
            {
            }

        public:
            virtual _Type const &getCurrent(void) const
            {
                if(this->_begin == NULL)
                {
                    //TODO print error msg
                    //throw std::runtime_error("Begin iterator is null.");
                }

                if(this->_end == NULL)
                {
                    //TODO print error msg
                    //throw std::runtime_error("End iterator is null.");
                }

                if(this->_current == NULL)
                {
                    //TODO print error msg
                    //throw std::out_of_range("Iterator is out of range. Please use moveNext() method.");
                }

                if(*this->_current == *_end)
                {
                    //TODO print error msg
                    //throw std::out_of_range("Iterator is out of range. Please use reset() method.");
                }

                return _converter(*this->_current);
            }

            virtual _Type const &operator*() const
            {
                return getCurrent();
            }

            virtual bool moveNext(void) const
            {
                if (this->_begin == NULL)
                {
                    //TODO:add error log
                    //throw std::runtime_error("Begin iterator is null.");
                    std::cout<<"Begin iterator is null."<<std::endl;
                }

                if (this->_end == NULL )
                {
                    //TODO:add error log
                    //throw std::runtime_error("End iterator is null.");
                    std::cout<<"End iterator is null."<<std::endl;
                }

                if (this->_current == NULL)
                {
                    this->_current = new _IteratorType(*_begin);
                }
                else if (*this->_current == *_end)
                {
                    return false;
                }
                else
                {
                    ++(*this->_current);
                }

                if (*this->_current == *_end)
                {
                    return false;
                }

                return true;
            }

            virtual void reset(void) const
            {
                if(this->_current == NULL)
                {
                    delete this->_current;
                }

                this->_current = NULL;
            }

            virtual void operator()(_IteratorType const &begin, _IteratorType const &end) const
            {
                if(this->_begin != NULL)
                {
                    delete this->_begin;
                }

                if(this->_end != NULL)
                {
                    delete this->_end;
                }

                if(this->_current != NULL)
                {
                    delete this->_current;
                }

                this->_current = NULL;
                this->_begin = new _IteratorType(begin);
                this->_end = new _IteratorType(end);
            }

            Iterator &operator=(Iterator const &arg)
            {
                if (this->_begin != NULL )
                {
                    delete this->_begin;
                }

                if (this->_end != NULL )
                {
                    delete this->_end;
                }

                if (this->_current != NULL )
                {
                    delete this->_current;
                }

                this->_current = new _IteratorType(arg._current);
                this->_begin = new _IteratorType(arg._begin);
                this->_end = new _IteratorType(arg._end);

                return *this;
            }

        public:
            virtual ~Iterator(void)
            {
                if(this->_begin != NULL)
                {
                    delete this->_begin;
                }

                if(this->_end != NULL)
                {
                    delete this->_end;
                }

                if(this->_current != NULL)
                {
                    delete this->_current;
                }

                this->_begin = NULL;
                this->_end = NULL;
                this->_current = NULL;
            }
        };

        template<typename _Container,
                 typename _IteratorType = typename _Container::const_iterator>
        struct StdContainerRange
        {
            /**
             * Convert begin range of iterator.
             *
             * @param iterator The source iterator.
             * @return The result iterator.
             */
            inline _IteratorType begin(_Container const &iterator) const
            {
                return iterator.begin();
            }

            /**
            * Convert begin range of iterator.
            *
            * @param iterator The source iterator.
            * @return The result iterator.
            */
            inline _IteratorType end(_Container const &iterator) const
            {
                return iterator.end();
            }
        };

        template<typename _ContainerType,
                 typename _ValueType = typename _ContainerType::value_type,
                 typename _IteratorType = typename _ContainerType::const_iterator,
                 typename _ConverterType = DefaultIteratorConverter<_ValueType, _IteratorType>,
                 typename _ContainerRangeType = StdContainerRange<_ContainerType, _IteratorType> >

        class StdContainerAggregate : public virtual IAggregate<_ValueType>
        {
        public:
            typedef _ContainerType ContainerType;
            typedef _ValueType ValueType;
            typedef _IteratorType IteratorType;
            typedef _ConverterType ConverterType;
            typedef _ContainerRangeType ContainerRangeType;
            typedef typename Interfaces::IAggregate<_ValueType> Aggregate;

        private:
            ContainerType _container;
            ContainerRangeType _container_range;

        private:
            template<typename _Type>
            static inline void setNull(_Type const &arg){}

            template<typename _Type>
            static inline void setNull(_Type* &arg)
            {
                arg = NULL;
            }

            template<typename _Type>
            static inline void setNull(_Type const* &arg)
            {
                arg = NULL;
            }

        public:
            class IteratorImplement : public Patterns::Iterator<ValueType, IteratorType, ConverterType>
            {
                friend class StdContainerAggregate<ContainerType, ValueType, IteratorType, ConverterType, ContainerRangeType>;
            };

        public:
            /**
             * Default constructor.
             */
            explicit StdContainerAggregate(void)
            {
            }

            /**
             * Constructor with constant reference to stl-container.
             *
             * @param container the stl-iterator reference.
             */
            explicit StdContainerAggregate(ContainerType const &container)
                : _container(container)
            {
            }

            /**
             * Copy constructor.
             *
             * @param arg the target argument to be copy from.
             */
            explicit StdContainerAggregate(StdContainerAggregate const &arg)
                : _container(arg._container)
            {
            }

        public:
            /**
             * Assignment operator support.
             *
             * @param arg the target augment to be copy from.
             * @return The this reference.
             */
            StdContainerAggregate &operator=(StdContainerAggregate const &arg)
            {
                _container = arg._container;
                return *this;
            }

        public:
            /**
             * Get stl-container from aggregate.
             *
             * @return The stl-container reference.
             */
            virtual ContainerType &get(void)
            {
                return _container;
            }
            /**
             * Get stl-container from aggregate.
             *
             * @return The stl-container reference.
             */
            virtual ContainerType const &get(void) const
            {
                return _container;
            }
        public:
            /**
             * Set iterator to iterator.
             *
             * @param iterator the stl-iterator.
             */
            virtual void setIterator(IIterator<ValueType> &iterator) const
            {
                typename StdContainerAggregate::IteratorImplement *result;
                result = dynamic_cast<typename StdContainerAggregate::IteratorImplement*>(&iterator);
                setIterator(*result);
            }

            /**
             * Set iterator to iterator.
             *
             * @param iterator the stl-iterator.
             */
            virtual void setIterator(typename StdContainerAggregate<ContainerType, ValueType, IteratorType, ConverterType, ContainerRangeType>::IteratorImplement &iterator) const
            {
                iterator(_container_range.begin(_container), _container_range.end(_container));
            }

            /**
             * Get iterator from this.
             *
             * @return The auto pointer of iterator.
             */
            virtual std::auto_ptr<IIterator<ValueType> > getIterator(void) const
            {
                std::auto_ptr<IIterator<ValueType> >

                result(new typename StdContainerAggregate::IteratorImplement());

                setIterator(*result);

                return result;
            }

            /**
             * Virtual destructor.
             */
            virtual ~StdContainerAggregate()
            {
            }
        };
    }
}

#endif //_ITERATOR_H_
