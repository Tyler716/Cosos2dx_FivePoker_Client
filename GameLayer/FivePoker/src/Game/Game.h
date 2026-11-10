#ifndef _GAME_H_
#define _GAME_H_

#include "Interfaces/IGame.h"
#include "UI_cocos2dx/FivePoker/AppDelegate.h"
#include "facade.h"

namespace GameLayer
{
    namespace Game
    {
        using GameLayer::Interfaces::IGame;
        using namespace BaseLayer::Patterns;

        class Game : public IGame
        {
        public:
            Game();

            bool initialize();

            void destroyGame();

            void exec();

            virtual ~Game();

            void setWindowSize(int iWidth, int iHeight);

        private:
            IFacade *facade;
            int m_iWidth, m_iHeight;
        };
    }
}

#endif //_GAME_H_
