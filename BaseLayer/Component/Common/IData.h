#ifndef _DATAOBJECT_H_
#define _DATAOBJECT_H_

#include <string>
#include <map>

namespace BaseLayer
{
    namespace Common
    {
        struct IData
        {
        public:
            IData(){};
            virtual ~IData(){};

        };
    }
}

#endif //_DATAOBJECT_H_
