#include "SocketProxy.h"
#include "../../Component/Patterns/Observer/Observer.h"
#include "Event.h"
#include "Common.h"
#include "IData.h"
#include "Type.h"
#include "../../Core/View/View.h"
#include <map>

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib, "ws2_32")
#define WSA_VERSION MAKEWORD(2, 2) // using winsock 2.2
#else
#include <pthread.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif

#if defined(_WIN32) || defined(_WIN64)
#define sleepTime   0
#else
#define sleepTime   100000
#endif  //defined(_WIN32) || defined(_WIN64)

using BaseLayer::Services::SocketProxy;
using BaseLayer::Patterns::Observer;
using BaseLayer::Interfaces::IObserver;
using BaseLayer::Interfaces::IView;
using BaseLayer::Core::View;
using BaseLayer::Common::IData;
using BaseLayer::NET::GameSocketClient;
using BaseLayer::NET::SocketClient;
using std::string;

SocketProxy::SocketProxy(std::string const &proxy_name /*= Proxy::NAME*/)
    : Proxy(proxy_name, NULL)
    , _client(NULL)
    , tid(NULL)
{
    if(!_data)
    {
        _data = (void *)new std::map<string, IData *>;
    }

    modelData = new ConnectionModelData();

    registerModelData("ConnectionModelData", (IData *)modelData);
}

SocketProxy::~SocketProxy()
{
    if(_client)
    {
#ifdef _DEBUG_CONSOLE_
    std::cout<<"delete _client"<<std::endl;
#endif

        delete _client;
        _client = NULL;
    }

#if defined(_WIN32) || defined(_WIN64)
    if(tid)
    {
        CloseHandle(tid);
        tid = NULL;
    }
#endif

    releaseModelData("ConnectionModelData");

    std::map<std::string, IData *> *datamap = (std::map<std::string, IData *> *)_data;
    if(datamap)
    {
        datamap->clear();
        delete datamap;
    }
}

#if defined(_WIN32) || defined(_WIN64)
DWORD SocketProxy::recvThread(void *arg)
{
#else
void *SocketProxy::recvThread(void *arg)
{
#endif
    //使thread結束後自行釋放資源
#if defined(_WIN32) || defined(_WIN64)
#else
    pthread_detach(pthread_self());
#endif  //defined(_WIN32) || defined(_WIN64)

#ifdef _DEBUG_CONSOLE_
    string strName = static_cast<SocketProxy *>(arg)->getProxyName();
#if defined(_WIN32) || defined(_WIN64)
    std::cout<<"Enter"<<__FUNCTION__<<"By Name:"<<strName<<std::endl;
#endif  //defined(_WIN32) || defined(_WIN64)
#endif  //_DEBUG_CONSOLE_

    ConnectionModelData *sockmodel = static_cast<SocketProxy *>(arg)->modelData;
    Byte *buf = NULL;

    if(sockmodel->_client)
    {
        if(STORE_SOCKET_BUF_LEN > sockmodel->_client->getHeaderSize())
        {
            buf = new Byte[STORE_SOCKET_BUF_LEN - sockmodel->_client->getHeaderSize()];
        }
        else
        {
            //error
        }
    }

    if(sockmodel && buf)
    {
#ifdef _DEBUG_CONSOLE_
        std::cout<<"state::"<<sockmodel->connectState<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "sockmodel->connectState:%i", sockmodel->connectState);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
        while(sockmodel->connectState == ConnectionModelData::CONNECT_SUCCESS)  //連線狀態OK
        {
            if(sockmodel->_client) //client socket存在
            {
                //if(sockmodel->_client->Receivable() == SocketClient::RET_SUCCESS)
                {
                    int iRecvState = sockmodel->_client->ReceiveData(buf);

                    if(iRecvState == SocketClient::RECV_COMPLETE) //接收資料
                    {
                        //讀完一個封包了
                        if(sockmodel->_client->getCommandID() != 1302 && sockmodel->_client->getCommandID() != 257)
                        {
#ifdef _DEBUG_CONSOLE_
                            std::cout<<"get pack, commandID:"<<sockmodel->_client->getCommandID()<<", content:"<<buf
                                     <<", size:"<<sockmodel->_client->getCommandSize()<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "get pack, commandID:%llu, pack size:%llu", 
                                                sockmodel->_client->getCommandID(),
                                                sockmodel->_client->getCommandSize());
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                        }

                        int iCommandType = sockmodel->_client->getCommandType();

                        switch(iCommandType)
                        {
                            case CT_SYS:
                            {
                                if(sockmodel->_client->getCommandID() != 1302 && sockmodel->_client->getCommandID() != 257)
                                {
#ifdef _DEBUG_CONSOLE_
                                    std::cout<<"get CT_SYS commandID:"<<sockmodel->_client->getCommandID()<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "get CT_SYS commandID:%llu", 
                                                        sockmodel->_client->getCommandID());
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                                }
                                sockmodel->_client->systemCommandProcess(sockmodel->_client->getCommandID(), buf);
                                break;
                            }

                            case CT_APP:
                            {
                                int iCmdID = sockmodel->_client->getCommandID();
                                string str = static_cast<SocketProxy *>(arg)->CommandIDToCommandName(iCmdID);

                                if(sockmodel->_client->getCommandID() != 1302 && sockmodel->_client->getCommandID() != 257)
                                {
#ifdef _DEBUG_CONSOLE_
                                    std::cout<<"get CT_APP commandID:"<<sockmodel->_client->getCommandID()<<", Name:"<<str<<std::endl;
#endif  //_DEBUG_CONSOLE_

                                    if(iCmdID != 1302 && iCmdID != 257)
                                    {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "get CT_APP commandID:%llu",    
                                                            sockmodel->_client->getCommandID());
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                                    }
                                }
                                if(str != "")
                                {
                                    int iCmdSize = sockmodel->_client->getCommandSize();
                                    
                                    if(strlen((const char*)buf) < iCmdSize)
                                    {
                                        buf[iCmdSize] = '\0';
                                    }
                                    
                                    if(iCmdID != 1302 && iCmdID != 257) //CS tick及GS tick
                                    {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                                        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "sendNotification:%s", 
                                                            str.data());
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                                    }
                                    static_cast<SocketProxy *>(arg)->sendNotification(str, buf);
                                    sockmodel->_client->resetCommand();
                                }
                                else
                                {
#ifdef _DEBUG_CONSOLE_
                                    std::cout<<"Can't Find CommandID:"<<sockmodel->_client->getCommandID()<<"in CommandList!"<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Can't Find CommandID::%i in CommandList!", 
                                                        sockmodel->_client->getCommandID());
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                                }
                                break;
                            }
                        }
                        //_client->recvPack(_client->getCommandID(), buf, _client->getCommandSize(), _client->getCommandType());
                    }
                    else if(iRecvState == SocketClient::RECV_ERROR)
                    {
                    }
                }
#if defined(_WIN32) || defined(_WIN64)
                //Sleep(sleepTime);
#else
                usleep(sleepTime); //0.8s
#endif  //defined(_WIN32) || defined(_WIN64)
            }
            else
            {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "sockmodel->_client doesn't exist!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
            }
        }

        delete [] buf;
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "sockmodel->connectState:%i", sockmodel->connectState);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
    }

#ifdef _DEBUG_CONSOLE_
#if defined(_WIN32) || defined(_WIN64)
    std::cout<<"Leave"<<__FUNCTION__<<std::endl;
#else
    std::cout<<"Leave"<<__func__<<std::endl;
#endif  //defined(_WIN32) || defined(_WIN64)
#endif  //_DEBUG_CONSOLE_
    return 0;
}

void SocketProxy::onRegister()
{
    _client = modelData->_client;

#if defined(_WIN32) || defined(_WIN64)
    DWORD dwThreadId;
    tid = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&SocketProxy::recvThread, (void *)(this), 0, (DWORD*)&dwThreadId);
#else
    pthread_create(&tid, NULL, &SocketProxy::recvThread, (void *)(this));
#endif

#ifdef _DEBUG_CONSOLE_
#if defined(_WIN32) || defined(_WIN64)
#else
    std::cout<<_proxy_name<<" pthread ID:"<<tid<<std::endl;
#endif  //defined(_WIN32) || defined(_WIN64)
#endif  //_DEBUG_CONSOLE_
}

void SocketProxy::onRemove()
{
    //TODO:等待thread結束再跑完
}
