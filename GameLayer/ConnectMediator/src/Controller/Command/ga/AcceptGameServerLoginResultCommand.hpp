#ifndef _ACCEPTGAMESERVERLOGINRESULTCOMMAND_HPP_
#define _ACCEPTGAMESERVERLOGINRESULTCOMMAND_HPP_

#include "Command.h"
#include "../../../Common/CommandList.h"
#include "../../../Model/vo/ga/GS_GA_RELOGIN_USER_R_Vo.h"
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class AcceptGameServerLoginResultCommand : public Command
        {
        public:
            AcceptGameServerLoginResultCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
                GS_GA_RELOGIN_USER_R_Vo *relogin_R_vo = (GS_GA_RELOGIN_USER_R_Vo *)note.getBody();

#ifdef _DEBUG_CONSOLE_
                std::cout<<"Login to GameServer state:"<<relogin_R_vo->state<<std::endl;
#endif
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Login to GameServer state:%i",relogin_R_vo->state);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                if(relogin_R_vo->state == GS_GA_RELOGIN_USER_R_Vo::STATE_FAIL)
                {
                    //TODO:
                }
            }
        };
    }
}

#endif //_ACCEPTGAMESERVERLOGINRESULTCOMMAND_HPP_
