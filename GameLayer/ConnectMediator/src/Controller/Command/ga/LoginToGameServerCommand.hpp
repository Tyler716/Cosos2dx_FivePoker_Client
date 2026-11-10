#ifndef _LOGINTOGAMESERVERCOMMAND_HPP_
#define _LOGINTOGAMESERVERCOMMAND_HPP_

#include "Command.h"
#include "../../../Common/CommandList.h"
#include "../../../Model/vo/ga/GS_GA_RELOGIN_USER_S_Vo.h"
#include "../../../Model/ModelData/GameServerLoginBufferModelData.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class LoginToGameServerCommand : public Command
        {
        public:
            LoginToGameServerCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get LoginToGameServerCommand!"<<std::endl;
#endif
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Get LoginToGameServerCommand!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                IProxy *GS_proxy = NULL;

                if(!getFacade().hasProxy("GameServerProxy"))
                {
                    //不存在proxy, print error log
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"GameServerProxy doesn't exist when LoginToGameServerCommand!"<<std::endl;
#endif
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "GameServerProxy doesn't exist when LoginToGameServerCommand!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                }
                else    //已存在
                {
                    GS_proxy = &getFacade().retrieveProxy("GameServerProxy");

                    if(GS_proxy != NULL)
                    {
                        std::map<std::string, IData *> *GS_datamap = (std::map<std::string, IData *> *)GS_proxy->getData();
                        ConnectionModelData *Connectmodeldata = (ConnectionModelData *)GS_datamap->find("ConnectionModelData")->second;
                        GameServerLoginBufferModelData *loginModeldata = (GameServerLoginBufferModelData *)GS_datamap->find("GameServerLoginBufferModelData")->second;

                        GS_GA_RELOGIN_USER_S_Vo relogin_S_vo;
                        relogin_S_vo.GSGrpNo = loginModeldata->loginData.GSGrpNo;
                        relogin_S_vo.GSGrpUniID = loginModeldata->loginData.GSGrpUniID;

                        //TODO:先寫死, relogin_S_vo還沒此欄位
                        relogin_S_vo.CardType = CARD_TYPE_MEMBER;

                        relogin_S_vo.GameType = loginModeldata->loginData.GameType;
                        relogin_S_vo.GameMode = loginModeldata->loginData.GameMode;
                        relogin_S_vo.SeatNo = loginModeldata->loginData.SeatNo;
                        relogin_S_vo.PLUniID = loginModeldata->loginData.PLUniID;

                        Login(Connectmodeldata->_client, &relogin_S_vo);
                    }
                    else
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"LoginToCentralServerCommand get LoginServerProxy fail!"<<std::endl;
#endif
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "LoginToCentralServerCommand get LoginServerProxy fail!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                    }
                }
            }

        private:
            void Login(GameSocketClient *client, GS_GA_RELOGIN_USER_S_Vo *loginVo)
            {
                client->SendData(GameServerEvent_map["GS_GA_RELOGIN_USER"],
                                 (Byte *)loginVo, sizeof(*loginVo));
            }
        };
    }
}

#endif //_LOGINTOGAMESERVERCOMMAND_HPP_
