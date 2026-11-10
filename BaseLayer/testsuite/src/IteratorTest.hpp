#ifndef _ITERATORTEST_HPP_
#define _ITERATORTEST_HPP_

#include "../../Component/Interfaces/IAggregate.h"
#include "../../Component/Patterns/Iterator/Iterator.h"

// C library include
#include <stdio.h>

// STL include
#include <string>
#include <list>

namespace data
{
    using BaseLayer::Interfaces::IIterator;
    using BaseLayer::Interfaces::IAggregate;

    struct IteratorTest : public IAggregate<std::string>
    {
        typedef std::list<std::string> list_string_t;
        std::list<std::string> listItem;

        struct Converter : public std::unary_function<list_string_t::const_iterator, std::string> {
            std::string const &operator()(list_string_t::const_iterator const &iterator) const;
        };
        class Iterator : public BaseLayer::Patterns::Iterator<std::string, list_string_t::const_iterator, Converter>
        {  friend struct IteratorTest;    };

        virtual void setIterator(IIterator<std::string> &iterator) const;

        virtual void setIterator(IteratorTest::Iterator &iterator) const;

        virtual IAggregate<std::string>::Iterator getIterator(void) const;
        };
}

#endif //_ITERATORTEST_HPP_
