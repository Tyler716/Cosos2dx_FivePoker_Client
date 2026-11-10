#include "GameSocketClient.h"
#include "../../Tools/CRCGenerator.h"
#include <cstring>

#if !defined(_WIN32) && !defined(_WIN64)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif  //!defined(_WIN32) && !defined(_WIN64)

using BaseLayer::NET::GameSocketClient;

/*
const int BaseLayer::NET::GameSocketClient::CT_SYS = 0;
const int BaseLayer::NET::GameSocketClient::CT_APP = 1;
*/
GameSocketClient::GameSocketClient(std::string ip /*= "127.0.0.1"*/,
                                   int port /*= 3000*/,
                                   bool autoLogin /*= true*/)
:SocketClient(ip, port)
{
    receivedCommandType = 0;
    receivedPacketSequence = 0;
    receivedDummySize = 0;
    receivedPacketCrc = 0;
    sentPacketCrc = 0;

    isAutoLogin = autoLogin;
}

void GameSocketClient::beforeEncryptContext(Byte *data, int iSize)
{
    //if(data != NULL)
    {
        sentPacketCrc = CRCGenerator::CalcCRC(data, iSize);
    }
}

void GameSocketClient::writePacketHeader(int id)
{
    int dummySize = sentCryptoPacketLength - sentPacketLength;
    int commandType = (id >= S2C_SCID_KEEPALIVE && id <= S2C_SCID_LOGIN_PERMISSION) ? CT_SYS : CT_APP;
    int sequence = 0;

    m_CRC = sentPacketCrc;
    m_DummySize = dummySize;
    m_CommandID = id;
    m_CommandSize = sentCryptoPacketLength + sizeof(m_ui64Header);
    m_CommandType = commandType;
    m_Sequence = sequence;

#ifdef _DEBUG_CONSOLE_
    if(id != 1302 && id != 257)
    {
        std::cout<<"Send from Client CRC:"<<sentPacketCrc<<", m_CommandID:"<<id
                 <<", m_CommandSize:"<<sentCryptoPacketLength + 8<<", m_CommandType:"<<commandType<<std::endl;
    }
#endif

    //把資料加密
    _Encrypter->Encrypt((char *)&m_ui64Header, sizeof(m_ui64Header));
}

int GameSocketClient::readPacketHeader(Byte *data, int dataSize, Byte *packbuf)
{
    if(data != NULL)
    {
        int headerLen = sizeof(m_ui64Header);

        if(dataSize < headerLen)
        {
            return -1;  //fail
        }
        int bufLen = dataSize - headerLen;

        //拆解檔頭
        UInt64 tmpHeader = 0;

        memcpy((void *)&tmpHeader, data, headerLen);
        _Encrypter->Decrypt((char *)&tmpHeader, headerLen);

        //TODO:確認怎麼分別big和little
/*
        m_CRC = (tmpHeader >> 16) & 0xFFFF;
        m_DummySize = (tmpHeader >> 13) & 0x7;
        m_CommandID = tmpHeader & 0x1FFF;
        m_CommandSize = ((tmpHeader >> 10) & 0x3FFFFF) - sizeof(m_ui64Header);
        m_CommandType = (tmpHeader >> 8) & 0x3;
        m_Sequence = tmpHeader & 0xFF;
*/
        //little_endian
        m_CRC = tmpHeader & 0xFFFF;
        m_DummySize = (tmpHeader >> 16) & 0x7;
        m_CommandID = (tmpHeader >> 19) & 0x1FFF;
        m_CommandSize = ((tmpHeader >> 32) & 0x3FFFFF) - sizeof(m_ui64Header);
        m_CommandType = (tmpHeader >> 54) & 0x3;
        m_Sequence = (tmpHeader >> 56) & 0xFF;

#ifdef _DEBUG_CONSOLE_
        if(m_CommandID != 1302 && m_CommandID != 257)
        {
            std::cout<<"readPacketHeader m_CommandID:"<<m_CommandID<<", m_CommandSize:"<<m_CommandSize<<", tmpHeader:"<<tmpHeader<<std::endl;
        }
#endif

        if(packbuf == NULL)
        {
            if(bufLen == 0)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            if(bufLen >= m_CommandSize)
            {
                memcpy(packbuf, data + headerLen, m_CommandSize);
            }
            else if(bufLen == 0)
            {
                return 0;
            }
            else if(bufLen < m_CommandSize)
            {
                return bufLen;
            }
        }

        return m_CommandSize;
    }

    return false;
}

bool GameSocketClient::systemCommandProcess(int commandID, Byte *data)
{
    switch(commandID)
    {
        case S2C_SCID_KEEPALIVE:
        {
            SendData(C2S_SCID_KEEPALIVE_ACK, NULL, 0);
            break;
        }

        case S2C_SCID_LOGIN_PERMISSION:
        {
            break;
        }

        case S2C_SCID_LOGOUT:
        {
            break;
        }
    }
    return (commandID >= S2C_SCID_KEEPALIVE && commandID <= S2C_SCID_LOGIN_PERMISSION);
}

bool GameSocketClient::recvPack(int commandID, Byte *data, int commandSize, int CommandType)
{
    if(CommandType == CT_SYS)
    {
        systemCommandProcess(commandID, data);
    }
    else
    {
        //TODO:需轉換commandID為command再送出
    }
    return false;
}
