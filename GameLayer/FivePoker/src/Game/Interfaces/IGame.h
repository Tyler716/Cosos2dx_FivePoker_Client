#ifndef _IGAME_H_
#define _IGAME_H_

namespace GameLayer
{
    namespace Interfaces
    {
        class IGame
        {
        public:
            virtual bool initialize() = 0;

            virtual void destroyGame() = 0;

            virtual void exec() = 0;

            virtual ~IGame(){};
        };
    }
}

#endif //_IGAME_H_
