#ifndef _ICOMMAND_H_
#define _ICOMMAND_H_

#include "INotifier.h"
#include "INotification.h"

namespace BaseLayer
{
    namespace Interfaces
    {
        class ICommand : public virtual INotifier
        {
        public:
            virtual void execute(INotification const &notification) = 0;

            virtual ~ICommand(void){};
        };
    }
}

#endif //_ICOMMAND_H_
