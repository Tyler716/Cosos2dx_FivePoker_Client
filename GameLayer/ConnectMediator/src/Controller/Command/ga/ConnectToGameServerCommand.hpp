#ifndef _CONNECTTOGAMESERVERCOMMAND_HPP_
#define _CONNECTTOGAMESERVERCOMMAND_HPP_

#include "Command.h"
#include "../../../Common/CommandList.h"
#include "../../../Model/Proxy/GameServerProxy.hpp"
#include "../../../Model/vo/cs/CS_LB_USER_RELOGIN_GS_R_Vo.h"
#include "../../../Model/ModelData/GameServerLoginBufferModelData.h"
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#include <dlfcn.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class ConnectToGameServerCommand : public Command
        {
        public:
            ConnectToGameServerCommand()
                : Command()
            {
            }

            virtual void execute(INotification const &note)
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Get ConnectToGameServerCommand!"<<std::endl;
#endif
                IProxy *GS_proxy = NULL;

                CS_LB_USER_RELOGIN_GS_R_Vo *vo = (CS_LB_USER_RELOGIN_GS_R_Vo *)note.getBody();

                if(!getFacade().hasProxy("GameServerProxy"))
                {
                    GS_proxy = new GameServerProxy("GameServerProxy");
                    std::map<std::string, IData *> *Gs_datamap = (std::map<std::string, IData *> *)GS_proxy->getData();

                    ConnectionModelData *modeldata = (ConnectionModelData *)Gs_datamap->find("ConnectionModelData")->second;
                    GameServerLoginBufferModelData *loginModeldata = (GameServerLoginBufferModelData *)Gs_datamap->find("GameServerLoginBufferModelData")->second;

                    if(loginModeldata)
                    {
                        loginModeldata->loginData = *vo;
                    }

                    if(modeldata)
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"ConnectToGameServerCommand vo:"<<modeldata<<std::endl;
#endif
                        if(vo)
                        {
                            char ip[100];
                            memset(ip, 0, sizeof(ip));
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

                            void* pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);
                            int (*ucnv_convert)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*) = 0;

                            if(!pDL)
                            {
                                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Can't load libicuuc.so!");
                            }
                            else
                            {
                                std::string strVer[5];
                                strVer[0] = std::string("_3_8");
                                strVer[1] = std::string("_4_2");
                                strVer[2] = std::string("_44");
                                strVer[3] = std::string("_46");
                                strVer[4] = std::string("_48");
                                std::string strConvert = std::string("ucnv_convert");
                                std::string strFull;

                                for(int i = 0; i < 6; i++)
                                {
                                    if(i == 0)
                                    {
                                        strFull = strConvert;
                                    }
                                    else
                                    {
                                        strFull = strConvert + strVer[i-1];
                                    }

                                    if(!ucnv_convert)
                                    {
                                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "try %s", strFull.data());
                                        ucnv_convert = (int (*)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*))dlsym(pDL, strFull.data());
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }

                                dlclose(pDL);
                                pDL = NULL;

                                if(ucnv_convert)
                                {
                                    //wcstombs(ip, vo->IP, wcslen(vo->IP));
                                    int err_code = 0;
                                    int ilen = ucnv_convert("utf-8",sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE", 
                                                            (char *)ip, sizeof(ip), 
                                                            (const char *)vo->IP, wcslen(vo->IP)*sizeof(wchar_t), 
                                                            &err_code);
                                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "wcslen(vo->IP):%i, ip:%s ip0:%c, len:%i, error:%i", wcslen(vo->IP), ip, ip[0], ilen, err_code);
                                }
                                else
                                {
                                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "ucnv_convert fail!");
                                }
                            }
                            
                            modeldata->ip = ip;
#endif	//CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#else
                            wcstombs(ip, vo->IP, wcslen(vo->IP));
                            modeldata->ip = ip;
#endif	//!defined(_WIN32) && !defined(_WIN64)
                            modeldata->port = vo->PORT;
                            modeldata->autoLogin = false;
                        }
                        else
                        {
                            modeldata->ip = "127.0.0.1";
                            modeldata->port = 35163;
                            modeldata->autoLogin = true;
                        }

                        if(!modeldata->_client)
                        {
#ifdef _DEBUG_CONSOLE_
                            std::cout<<"ConnectToGameServerCommand _client is NULL!"<<std::endl;
                            std::cout<<"new client with IP:"<<modeldata->ip<<", port:"<<modeldata->port<<std::endl;
#endif
                            modeldata->_client = new GameSocketClient(modeldata->ip, modeldata->port);
                        }

                        if(Connect(modeldata))
                        {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Connect to GameServer success!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                            getFacade().registerProxy(GS_proxy);

                            sendNotification("GameServerConnect");
                        }
                        else
                        {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Connect to GameServer fail!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                            sendNotification("GAMEMSG_LOGIN_FAIL");
                            //TODO:連線失敗
                            delete getFacade().removeProxy("GameServerProxy");
                        }
                    }
                }
                else    //proxy已經存在, 判斷是否重連?
                {

                }
            }

        private:
            bool ConnectGameServer(ConnectionModelData *modeldata)
            {
                if(!modeldata->_client)
                {
                    modeldata->_client = new GameSocketClient(modeldata->ip, modeldata->port);

#ifdef _DEBUG_CONSOLE_
                    std::cout<<"ConnectGameServer _client is NULL, new a client with ip:"<<modeldata->ip<<", port:"<<modeldata->port<<std::endl;
#endif

                    if(modeldata->_client)
                    {
                        if(Connect(modeldata))
                        {
                            return true;
                        }
                    }
                }
                return false;
            }

            bool Connect(ConnectionModelData *modeldata)
            {
                if(!modeldata->_client->Connect())
                {
                    modeldata->connectState = ConnectionModelData::CONNECT_FAIL;
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"ConnectToGameServerCommand connect fail!"<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "ConnectToGameServerCommand connect fail!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                    return false;
                }
                else
                {
                    modeldata->connectState = ConnectionModelData::CONNECT_SUCCESS;
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"ConnectToGameServerCommand connect success!"<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "ConnectToGameServerCommand connect success!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                }
                return true;
            }
        };
    }
}

#endif //_CONNECTTOGAMESERVERCOMMAND_HPP_
