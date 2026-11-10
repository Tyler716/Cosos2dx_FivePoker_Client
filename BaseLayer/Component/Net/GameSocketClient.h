#ifndef _GAMESOCKETCLIENT_H_
#define _GAMESOCKETCLIENT_H_

#include "SocketClient.h"
#include "Type.h"
#include "Common.h"

namespace BaseLayer
{
    namespace NET
    {
        using NET::SocketClient;

        class GameSocketClient : public SocketClient
        {
        public:
            GameSocketClient(string ip = "127.0.0.1", int port = 3000, bool autoLogin = true);

        protected:
            /**
             * 加密前要做的事情
             * @param   data[in]:傳入目前檔案內容
             * @param   iSize[in]:傳入的檔案內容大小
             */
            virtual void beforeEncryptContext(Byte *data, int iSize);

            /**
             *寫入封包擋頭 由子類別實作
             *@param    id[in]:封包編號
             */
            virtual void writePacketHeader(int id);

            /**
             * 拆解封包檔頭
             * @param   data[in]:傳入的封包
             * @param   datasize[in]:傳入的封包大小
             * @param   packbuf[out]:拆解後傳出的封包內容
             * @return  封包去掉檔頭後的大小, 失敗為-1
             */
            virtual int readPacketHeader(Byte *data, int dataSize, Byte *packbuf);

        public:
            /**
             *系統命令處理
             *@param    commandID[in]:命令編號
             *@param    data[in]:命令內容
             */
            virtual bool systemCommandProcess(int commandID, Byte *data);

            /**
             * 封包接收結束 先處理已接收封包
             * @param   command[in]:封包號碼
             * @param   data[in]:傳入的封包
             * @param   commandsize[in]:傳入的封包大小
             * @param   commandType[in]:封包類型
             * @return  回傳是否成功
             */
            virtual bool recvPack(int commandID, Byte *data, int commandSize, int CommandType);

        protected:
            int receivedCommandType;
            int receivedPacketSequence;
            int receivedDummySize;
            int receivedPacketCrc;
            int sentPacketCrc;
            bool isAutoLogin;
        };
    }
}

#endif //_GAMESOCKETCLIENT_H_
