#ifndef _IOBSERVER_H_
#define _IOBSERVER_H_

#include "INotification.h"

// STL include
#include <string>

namespace BaseLayer
{
    namespace Interfaces
    {
        class IObserver
        {
        public:
            virtual void notifyObserver(INotification const &notification) = 0;

            virtual bool compareNotifyContext(void const *object) const = 0;

            virtual ~IObserver(void){};
        };
    }
}

#endif //_IOBSERVER_H_
