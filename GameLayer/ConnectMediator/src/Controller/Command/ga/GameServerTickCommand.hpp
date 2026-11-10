#ifndef _GAMESERVERTICKCOMMAND_HPP_
#define _GAMESERVERTICKCOMMAND_HPP_

#include "Command.h"
#include "ConnectionModelData.h"
#include "../../../Common/CommandList.h"
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <signal.h>
#include <time.h>
#include <sys/errno.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#else
#include <Windows.h>
#endif  //!defined(_WIN32) && !defined(_WIN64)

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Interfaces::INotification;

        class GameServerTickCommand : public Command
        {
        public:
            GameServerTickCommand()
                : Command()
            {
                m_GSclient = NULL;
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                tid = 0;
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#else
                m_htid = NULL;
#endif  //!defined(_WIN32) && !defined(_WIN64)
            }

            virtual ~GameServerTickCommand()
            {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "delete timer");
                TickEnabled(false);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#else
                OutputDebugString(L"destruct GameServerTickCommand!\r\n");
                if(m_htid)
                {
                    OutputDebugString(L"close timeThread handle!\r\n");
                    CloseHandle(m_htid);
                    m_htid = NULL;
                }
#endif  //!defined(_WIN32) && !defined(_WIN64)
            }

            virtual void execute(INotification const &note)
            {
                if(!getFacade().hasProxy("GameServerProxy"))
                {
                    //不存在proxy, print error log
#ifdef _DEBUG_CONSOLE_
                    std::cout<<"GameServerProxy doesn't exist when GameServerTickCommand!"<<std::endl;
#endif
                }
                else    //已存在
                {
                    if(!m_GSclient)
                    {
                        IProxy *GS_proxy = NULL;
                        GS_proxy = &getFacade().retrieveProxy("GameServerProxy");

                        if(GS_proxy != NULL)
                        {
                            std::map<std::string, IData *> *GS_datamap = (std::map<std::string, IData *> *)GS_proxy->getData();
                            ConnectionModelData *Connectmodeldata = (ConnectionModelData *)GS_datamap->find("ConnectionModelData")->second;
                            m_GSclient = Connectmodeldata->_client;
                        }
                    }
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    if(tid == 0)
                    {
                        TickEnabled(true);
                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "enable GS Tick timer!");
                    }
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#else
                    //m_GSclient->SendData(GameServerEvent_map["GS_GA_TICK"], NULL, 0);
                    if(!m_htid)
                    {
                        OutputDebugString(L"create timeThread!\r\n");
                        DWORD dwThreadId;
                        m_htid = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&GameServerTickCommand::timerTickThread, (void *)this, 0, (DWORD*)&dwThreadId);
                    }
#endif  //!defined(_WIN32) && !defined(_WIN64)
                }
            }
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        private:
            void TickEnabled(bool bEnable)
            {
                if(bEnable)
                {
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "TickEnabled1 error:%i %s", errno, strerror(errno));

                    sigevent sigev;
                    memset(&sigev, 0, sizeof(sigev));
                    sigev.sigev_notify = SIGEV_THREAD;
                    sigev.sigev_notify_function = &GameServerTickCommand::onTick;
                    sigev.sigev_value.sival_ptr = this;

                    tid = 0;
                    if(timer_create(CLOCK_REALTIME, &sigev, &tid) != 0)
                    {
                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Timer create fail error:%i %s", errno, strerror(errno));
                    }
                    else
                    {
                        //TODO:timer_create get errno 22 Invalid argument
                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "timer_create error:%i %s", errno, strerror(errno));
                        errno = 0;
                    }

                    itimerspec ts;
                    memset(&ts, 0, sizeof(ts));

                    ts.it_interval.tv_sec = 5;
                    ts.it_interval.tv_nsec = 0;
                    ts.it_value = ts.it_interval; //5秒觸發一次

                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "set timer!");
                    timer_settime(tid, 0, &ts, NULL);
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "TickEnabled3 error:%i %s", errno, strerror(errno));
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                }
                else
                {
                    if(tid != 0)
                    {
                        timer_delete(tid);
                    }
                    tid = 0;
                }
            }

            static void onTick(sigval_t v)
            {
                //TODO:
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "send GS Tick!");
                static_cast<GameServerTickCommand *>(v.sival_ptr)->m_GSclient->SendData(GameServerEvent_map["GS_GA_TICK"], NULL, 0);
            }

            timer_t tid;
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#else
        private:
            static DWORD timerTickThread(void *arg)
            {
                GameSocketClient *gsclient = ((GameServerTickCommand *)arg)->m_GSclient;
                HANDLE h_tid = ((GameServerTickCommand *)arg)->m_htid;
                while(h_tid)
                {
                    Sleep(5000);
                    gsclient->SendData(GameServerEvent_map["GS_GA_TICK"], NULL, 0);
                    h_tid = ((GameServerTickCommand *)arg)->m_htid;
                }

                OutputDebugString(L"timerTickThread end!\r\n");
                return 0;
            }

            HANDLE m_htid;
#endif  //!defined(_WIN32) && !defined(_WIN64)

        private:
            GameSocketClient *m_GSclient;
        };
    }
}

#endif //_GAMESERVERTICKCOMMAND_HPP_
