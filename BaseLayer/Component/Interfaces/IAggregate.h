#ifndef _IAGGREGATE_H_
#define _IAGGREGATE_H_

#include "IIterator.h"

// STL include
#include <memory>
#include <utility>

namespace BaseLayer
{
    namespace Interfaces
    {
        using Interfaces::IIterator;

        template<typename _Type>
        class IAggregate
        {
        public:
            typedef std::auto_ptr<IIterator<_Type> > Iterator;

            virtual void setIterator(IIterator<_Type> &iterator) const = 0;

            virtual Iterator getIterator(void) const = 0;

            virtual ~IAggregate(){};
        };
    }
}

#endif //_IAGGREGATE_H_
