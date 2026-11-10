#include "SocketClient.h"

#include "../../Tools/RSAEncryption.h"
#include "Type.h"
#include <sys/types.h>
#include <bitset>
#include <cstring>
#include <iostream>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#define WSA_VERSION MAKEWORD(2, 2) // using winsock 2.2
#else
#include <arpa/inet.h>
#include <sys/errno.h>
#include <sys/select.h>
//#include <fcnt1.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif

using namespace BaseLayer::NET;
using std::string;

SocketClient::SocketClient(string ip /*= "127.0.0.1"*/, int port /*= 3000*/, bool bBlock /*= false*/)
{
    //initialize variables
#if defined(_WIN32) || defined(_WIN64)
    WSADATA	WSAData = {0};
    if (WSAStartup(WSA_VERSION, &WSAData) != 0)
    {
        // Tell the user that we could not find a usable WinSock DLL.
        if (LOBYTE(WSAData.wVersion) != LOBYTE(WSA_VERSION) ||
            HIBYTE(WSAData.wVersion) != HIBYTE(WSA_VERSION))
        {
            std::cout<<"Incorrect winsock version"<<std::endl;
        }

        WSACleanup();

        return;
    }
 #endif

    _sockfd= socket(AF_INET, SOCK_STREAM, 0);

    if(_sockfd < 0)
    {
#ifdef _DEBUG_CONSOLE_
        std::cout<<"construct socket fail!"<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "construct socket fail!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
    }
    else
    {
        m_bBlock = bBlock;

        if(!m_bBlock)
        {
#if defined(_WIN32) || defined(_WIN64)
            unsigned long nNonBlocking = 1;
            if (ioctlsocket(_sockfd, FIONBIO, &nNonBlocking) == SOCKET_ERROR)
            {
#ifdef _DEBUG_CONSOLE_
            std::cout<<"Unable to set nonblocking mode, ec:"<<WSAGetLastError()<<std::endl;
#endif  //_DEBUG_CONSOLE_
                Close();
                return;
            }
#else
            /*
            int flags = fcntl(_sockfd, F_GETFL, 0);
            fcntl(_sockfd, F_SETFL, flags|O_NONBLOCK);
            */
#endif  //defined(_WIN32) || defined(_WIN64)
        }

        _Encrypter = new RSAEncrypter();

        if(_Encrypter == NULL)
        {
            //TODO:print error log
        }
        else
        {
            _Encrypter->SetKey((void *)EncrypterKey);
        }

        _connectIP = ip;
        _connectPort = port;

        receivedCommandID = 0;
        receivedPacketLength = 0;
        sentPacketLength = 0;
        sentCryptoPacketLength = 0;
        _packIndex = 0;
        _isPacketContinue = false;
        _isHeaderFinished = false;
        m_iReceiveBytes = 0;

        memset(_RecvBuf, 0, sizeof(_RecvBuf));
    }
};

SocketClient::~SocketClient()
{
    SAFE_DELETE(_Encrypter);
}

string SocketClient::getConnectIP()
{
    return _connectIP;
}

void SocketClient::setConnectIP(string ip)
{
    _connectIP = ip;
}

int SocketClient::getConnectPort()
{
    return _connectPort;
}

void SocketClient::setConnectPort(int port)
{
    _connectPort = port;
}

bool SocketClient::Connect()
{
    sockaddr_in clientAddr;

#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SocketClient connect to IP:%s",_connectIP.data());
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(_connectPort);  //host to network short, little-endian
    clientAddr.sin_addr.s_addr = inet_addr(_connectIP.data());  //little-endian

    if(connect(_sockfd, (sockaddr *)&clientAddr, sizeof(clientAddr)) == -1)
    {
#if _DEBUG_CONSOLE_
        std::cout<<"connect fail, error:"<<WSAGetLastError()<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SocketClient connect fail, error:%s, errno:%i",strerror(errno), errno);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
    }
    else
    {
        std::cout<<"connect success, _sockfd:"<<_sockfd<<std::endl;

        return true;
    }

    return false;
}

/**
 * 關閉連線，通知對方此Socket不再傳送訊息了
 */
bool SocketClient::Shutdown()
{
    //release fd
    if(_sockfd == -1)
    {
#ifdef _DEBUG_CONSOLE_
        std::cout<<"Shutdown socket fail!"<<std::endl;
#endif
    }
    else
    {
#if defined(_WIN32) || defined(_WIN64)
        shutdown(_sockfd, SD_SEND); //不再SEND
#else
        shutdown(_sockfd, SHUT_WR); //不再SEND
#endif
        return true;
    }
    return false;
}

bool SocketClient::Close()
{
#if defined(_WIN32) || defined(_WIN64)
    if(closesocket(_sockfd) == -1)
    {
#else
    if(close(_sockfd) == -1)
    {
#endif  //defined(_WIN32) || defined(_WIN64)
#ifdef _DEBUG_CONSOLE_
        std::cout<<"close socket fail!"<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "close socket fail!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
    }
    else
    {
#ifdef _DEBUG_CONSOLE_
        std::cout<<"close socket success..."<<std::endl;
#endif  //_DEBUG_CONSOLE_
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "close socket success...");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
        _sockfd = -1;
#if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
#endif  //defined(_WIN32) || defined(_WIN64)
        return true;
    }

    return false;
}

/**
 * 送出資料，先加密後加上檔頭再送出
 */
void SocketClient::SendData(int id, Byte *buf, int iSize)
{
    if(id != 1302 && id != 257)
    {
#ifdef _DEBUG_CONSOLE_
        std::cout<<"Send Command:"<<id<<", size:"<<iSize<<std::endl;
#endif
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Send Command:%i, size:%i", id, iSize);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
    }
    //加密前處理
    //if(buf != NULL)
    {
        beforeEncryptContext(buf, iSize);
    }

    if(buf == NULL || iSize == 0)
    {
        sentPacketLength = 0;
        sentCryptoPacketLength = 0;
    }
    else
    {
        sentPacketLength = iSize;

        //處理加密
        _Encrypter->Encrypt((char *)buf, iSize);
        sentCryptoPacketLength = iSize;
    }

    if(sentCryptoPacketLength < 0)
    {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Encryp error! sentCryptoPacketLength:%i", sentCryptoPacketLength);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
    }
    else
    {
        //寫入檔頭
        writePacketHeader(id);

        int bufHeaderLength = sizeof(m_ui64Header);

        int bufLength = bufHeaderLength + sentCryptoPacketLength;
        Byte *bufTmp = new Byte[bufLength];

        memcpy(bufTmp, (void *)&m_ui64Header, bufHeaderLength);

        if(sentCryptoPacketLength > 0)
        {
            memcpy(bufTmp + bufHeaderLength, buf, sentCryptoPacketLength);
        }
#if defined(_WIN32) || defined(_WIN64)
        int sendLength = send(_sockfd, (const char *)bufTmp, bufLength, 0);
#else
        int sendLength = send(_sockfd, bufTmp, bufLength, 0);
#endif
        delete [] bufTmp;
        bufTmp = NULL;

        if(bufLength != sendLength)
        {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "bufLength %i doen't equal to sendLength:%i", 
                                bufLength,
                                sendLength);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#else
            std::cout<<"bufLen:"<<bufLength<<", doesn't equal to sendLen:"<<sendLength<<std::endl;
#endif  //!defined(_WIN32) && !defined(_WIN64)

        }
    }
}

int SocketClient::ReceiveData(void *data)
{
    int iRecvLen = 0;
    static int iHeaderLen = sizeof(m_ui64Header);  //取得檔頭大小

    if(_sockfd == -1)
    {
#if defined(_WIN32) || defined(_WIN64)
        std::cout<<__FUNCTION__<<"fail!"<<std::endl;
#else
        std::cout<<__func__<<"fail!"<<std::endl;
#endif
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "_sockfd:%i", _sockfd);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
    }
    else
    {
#ifdef _DEBUG_CONSOLE_
        if(strlen((const char *)_RecvBuf) != m_iReceiveBytes)
        {
            std::cout<<"RecvBuf長度:"<<strlen((const char *)_RecvBuf)
                     <<"與已接收長度:"<<m_iReceiveBytes<<"不同"<<std::endl;
        }
#endif

        //buffer沒滿就繼續收
        if(m_iReceiveBytes < RECV_SOCKET_BUF_LEN)
        {
            if(Receivable() == SocketClient::RET_SUCCESS)
            {
                iRecvLen = recv(_sockfd, (char *)_RecvBuf + m_iReceiveBytes,
                                RECV_SOCKET_BUF_LEN - m_iReceiveBytes, 0);
            }

            if(iRecvLen > 0)
            {
#if defined(_WIN32) || defined(_WIN64)
                wchar_t szLog[100];
                wsprintf(szLog, L"RecvLen:%i\r\n", iRecvLen);
                OutputDebugString(szLog);
#endif
                m_iReceiveBytes += iRecvLen;
            }
        }

        if(m_iReceiveBytes > 0)    //buffer有長度
        {
            int iDataLen = 0;
            int iPackLen = 0;
            Byte *tmpPackBuf = NULL;    //暫存每次接收封包去除檔頭後內容

#if defined(_WIN32) || defined(_WIN64)
            wchar_t szLog[100];
            wsprintf(szLog, L"m_iReceiveBytes:%i, receivedCommandID:%i\r\n", m_iReceiveBytes, receivedCommandID);
            OutputDebugString(szLog);
#endif

            if(receivedCommandID == 0)  //目前沒有正在接收的封包
            {
                if(m_iReceiveBytes >= iHeaderLen)    //檔頭收完就送去拆解
                {
                    iDataLen = m_iReceiveBytes - iHeaderLen;    //扣掉檔頭後剩餘大小

                    if(iDataLen == 0)  //只有檔頭沒內容
                    {
#ifdef _DEBUG_CONSOLE_
                        std::cout<<"only header!"<<std::endl;
#endif
                    }
                    else if(iDataLen > 0)
                    {
                        tmpPackBuf = new Byte[iDataLen];
                        memset(tmpPackBuf, 0, iDataLen);
                    }

                    iPackLen = readPacketHeader(_RecvBuf, m_iReceiveBytes, tmpPackBuf);

#if defined(_WIN32) || defined(_WIN64)
                    wchar_t szLog[100];
                    wsprintf(szLog, L"readPacketHeader get:%i, CommandID:%i, CommandSize:%i\r\n", iPackLen, m_CommandID, m_CommandSize);
                    OutputDebugString(szLog);
#endif

                    if(iPackLen == -1)
                    {
                        //TODO:print error log

                        if(tmpPackBuf)
                        {
                            delete [] tmpPackBuf;
                            tmpPackBuf = NULL;
                        }

                        return RECV_ERROR;
                    }
                    else if(iPackLen == 0)  //只有header,通常是tick
                    {
                        receivedPacketLength = iPackLen;

                        //只有檔頭沒內容,處理接收資料buffer
                        adjustBuffer(_RecvBuf, m_iReceiveBytes, iHeaderLen);

                        receivedCommandID = 0;

                        if(tmpPackBuf)
                        {
                            delete [] tmpPackBuf;
                            tmpPackBuf = NULL;
                        }

                        return RECV_COMPLETE;
                    }
                    else if(iPackLen > 0)
                    {
                        if(iPackLen < m_CommandSize)    //第一包沒收完
                        {
                            receivedCommandID = m_CommandID;
                            receivedPacketLength = iPackLen;
                            memcpy(data, tmpPackBuf, receivedPacketLength);
                            adjustBuffer(_RecvBuf, m_iReceiveBytes, iHeaderLen+iPackLen);

                            if(tmpPackBuf)
                            {
                                delete [] tmpPackBuf;
                                tmpPackBuf = NULL;
                            }
                            return RECV_UNCOMPLETE;
                        }
                        else if(m_iReceiveBytes >= iHeaderLen+iPackLen) //一次收完超過一個封包
                        {
                            receivedCommandID = 0;
                            receivedPacketLength = iPackLen;
                            memcpy(data, tmpPackBuf, iPackLen);
                            adjustBuffer(_RecvBuf, m_iReceiveBytes, iHeaderLen+iPackLen);

                            if(tmpPackBuf)
                            {
                                delete [] tmpPackBuf;
                                tmpPackBuf = NULL;
                            }
                        }
                    }
                }
                else    //檔頭都還沒收完, 繼續收
                {
                    return RECV_UNCOMPLETE;
                }
            }
            else    //狀態為繼續接收上一個封包
            {
                if((receivedPacketLength + m_iReceiveBytes) >= m_CommandSize)   //收完一包
                {
                    if(receivedPacketLength < m_CommandSize)
                    {
                        iDataLen = m_CommandSize - receivedPacketLength;

                        memcpy((void *)((Byte *)data+receivedPacketLength), _RecvBuf, iDataLen);
                        receivedPacketLength += iDataLen;
                        //處理接收資料buffer
                        adjustBuffer(_RecvBuf, m_iReceiveBytes, iDataLen);

                        iPackLen = m_CommandSize;
                        receivedCommandID = 0;
                    }
                    else
                    {
                        //error
                        return RECV_ERROR;
                    }
                }
                else    //還沒收完繼續
                {
                    receivedCommandID = m_CommandID;

                    memcpy((void *)((Byte *)data+receivedPacketLength), _RecvBuf, m_iReceiveBytes);
                    receivedPacketLength += m_iReceiveBytes;
                    //處理接收資料buffer
                    adjustBuffer(_RecvBuf, m_iReceiveBytes, m_iReceiveBytes);

                    return RECV_UNCOMPLETE;
                }
            }

#if defined(_WIN32) || defined(_WIN64)
            wsprintf(szLog, L"receivedPacketLength:%i, CommandID:%i, CommandSize:%i\r\n", receivedPacketLength, m_CommandID, m_CommandSize);
            OutputDebugString(szLog);
#endif

            if(receivedPacketLength == m_CommandSize)
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Decrypt pack Lengh:"<<receivedPacketLength<<std::endl;
#endif
                _Encrypter->Decrypt((char *)data, iPackLen);

                return RECV_COMPLETE;
            }
            else
            {
                //error
                return RECV_ERROR;
            }
        }
        else    //沒東西
        {
            if(!IsError())
            {
                return RECV_NOTHING;
            }
            else
            {
#ifdef _DEBUG_CONSOLE_
                std::cout<<"Socket receive Error! ErrorCode:"<<strerror(errno)<<std::endl;
#endif
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Socket receive Error! ErrorCode:%i %s", errno, strerror(errno));
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
                return RECV_ERROR;
            }
        }
    }

    return 0;
}

int SocketClient::Sendable()
{
    if(_sockfd == -1)
    {
        return RET_NOTHING;
    }

    fd_set fd_w, fd_excep;

    FD_ZERO(&fd_w);
    FD_SET(_sockfd, &fd_w);
    FD_ZERO(&fd_excep);
    FD_SET(_sockfd, &fd_excep);

    timeval timeout = {0};

    int cnt = select(_sockfd, NULL, &fd_w, &fd_excep, &timeout);

    if(cnt > 0)
    {
        if(FD_ISSET( _sockfd, &fd_excep))
        {
            if(!IsError())//Cary: Don't remove this line.
            {
                return RET_NOTHING;
            }

            //TODO:print error log
            //PrintNormalLog( Text( "Socket (%d) select sendable get an exception(%d)" ), m_uiSockfd, m_iErrNo );
            return RET_ERROR;
        }

        return RET_SUCCESS;
    }

    if(cnt < 0)
    {
        if(!IsError())//Cary: Don't remove this line.
        {
            return RET_NOTHING;
        }

        //TODO:print error log
        //PrintNormalLog( Text( "Socket (%d) select sendable get an error(%d)" ), m_uiSockfd, m_iErrNo );
        return RET_ERROR;
    }

    return RET_NOTHING;
}

int SocketClient::Receivable()
{
    if(_sockfd == -1)
    {
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "_sockfd == -1");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
        return RET_NOTHING;
    }

    fd_set fd_r, fd_excep;

    FD_ZERO (&fd_r);
    FD_SET(_sockfd, &fd_r);
    FD_ZERO(&fd_excep);
    FD_SET(_sockfd, &fd_excep);

    timeval timeout = {0};

    int cnt = select(_sockfd+1, &fd_r, NULL, &fd_excep, &timeout);

    if(cnt > 0)
    {
        if(FD_ISSET(_sockfd, &fd_excep))
        {
            if(!IsError())//Cary: Don't remove this line.
            {
                return RET_NOTHING;
            }

#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Receivable error:%i %s, cnt:%i", errno, strerror(errno), cnt);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
            return RET_ERROR;
        }
        else if(FD_ISSET(_sockfd, &fd_r))
        {
            return RET_SUCCESS;
        }
    }

    if(cnt < 0)
    {
        if(!IsError())//Cary: Don't remove this line.
        {
            return RET_NOTHING;
        }

        //TODO:print error log
#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Receivable error:%i %s, cnt:%i", errno, strerror(errno), cnt);
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)
        return RET_ERROR;
    }

    return RET_NOTHING;
}

bool SocketClient::IsError()
{
    if(errno == 0)
    {
        return false;
    }

    return errno != EWOULDBLOCK;
}

UInt64 SocketClient::getCrc()
{
    return m_CRC;
}

Int64 SocketClient::getDummySize()
{
    return m_DummySize;
}

UInt64 SocketClient::getCommandID()
{
    return m_CommandID;
}

UInt64 SocketClient::getCommandSize()
{
    return m_CommandSize;
}

UInt64 SocketClient::getCommandType()
{
    return m_CommandType;
}

UInt64 SocketClient::getSequence()
{
    return m_Sequence;
}

void SocketClient::resetCommand()
{
    m_ui64Header = 0;
}

int SocketClient::getHeaderSize()
{
    int iHeaderSize = sizeof(m_ui64Header);

    return iHeaderSize;
}

void SocketClient::adjustBuffer(Byte *data, int &bufferSize, int removeSize)
{
    if(bufferSize == 0)
    {
        return;
    }
    else if(bufferSize == removeSize)
    {
        memset(data, 0, bufferSize);
        bufferSize = 0;
        return;
    }

    Byte *tmpData = new Byte[bufferSize];
    memset(tmpData, 0, bufferSize);

    memcpy((void *)tmpData, (const void *)data, bufferSize);
    memset(data, 0, bufferSize);
    memcpy((void *)data, (const void *)(tmpData+removeSize), bufferSize - removeSize);

    bufferSize = bufferSize - removeSize;
}
