#ifndef _IITERATOR_H_
#define _IITERATOR_H_

namespace BaseLayer
{
    namespace Interfaces
    {
        template<typename _Type>
        class IIterator
        {
        public:
            virtual _Type const &getCurrent(void) const = 0;

            virtual _Type const &operator*(void) const = 0;

            virtual bool moveNext(void) const = 0;

            virtual void reset(void) const = 0;

            virtual ~IIterator(void){};
        };
    }
}

#endif //_IITERATOR_H_
