#ifndef _SOCKETCLIENT_H_
#define _SOCKETCLIENT_H_

#if defined(_WIN32) || defined(_WIN64)
#else
#include <sys/socket.h>
#endif
#include <string>
#include <iostream>
#include "Type.h"
#include "Common.h"
#include "../../Tools/Encryption.h"

namespace BaseLayer
{
    namespace NET
    {
        using std::string;
        using std::basic_istream;

        class SocketClient
        {
        public:
            /**
             * Socket客戶端連線類別
             * @param   ip[in]          連線位置
             * @param   port[in]        連線埠號
             */
            SocketClient(string ip = "127.0.0.1", int port = 3000, bool bBlock = true);

            virtual ~SocketClient();

            //TODO:
            //void setCryptoProvier(ICryptoProvider *value);
            //ICryptoProvider *getcryptoProvider();

            string getConnectIP();
            void setConnectIP(string ip);

            int getConnectPort();
            void setConnectPort(int port);

            bool Connect();

            bool Shutdown();

            bool Close();

            void SendData(int id, Byte *buf, int iSize);

            /**
             * 接收檔案
             * 回傳值:封包大小不含檔頭
             * RECV_ERROR: 錯誤
             * RECV_UNCOMPLETE:不完整的內容或是沒內容
             * RECV_HEADER: 只有檔頭, 通常為tick
             * RECV_COMPLETE: 完整封包
             */
            int ReceiveData(void *datas);

            /**
             * 檢查socket是否可送資料
             */
            int Sendable();

            /**
             * 檢查socket是否可送資料
             */
            int Receivable();

            bool IsError();

            UInt64 getCrc();
            Int64 getDummySize();
            UInt64 getCommandID();
            UInt64 getCommandSize();
            UInt64 getCommandType();
            UInt64 getSequence();
            int getHeaderSize();

            void resetCommand();

        public:
            enum
            {
                RET_SUCCESS,    // return a success
                RET_NOTHING,    // return do nothing
                RET_ERROR,      // return occur an error
            };

            enum
            {
                RECV_ERROR = 999,
                RECV_UNCOMPLETE = -1,
                RECV_NOTHING,
                RECV_HEADER,
                RECV_COMPLETE,
            };

        protected:
            /**
             * 加密前要做的事情
             * @param   data[in]:傳入目前檔案內容
             */
            virtual void beforeEncryptContext(Byte *data, int iSize) = 0;

            /**
             *寫入封包擋頭 由子類別實作
             *@param    id[in]:封包編號
             */
            virtual void writePacketHeader(int id) = 0;

            /**
             *系統命令處理
             *@param    commandID[in]:命令編號
             *@param    data[in]:命令內容
             */
            virtual bool systemCommandProcess(int commandID, Byte *data) = 0;

            /**
             * 拆解封包檔頭 由子類別實作
             * @param   data[in]:傳入的封包
             * @param   datasize[in]:傳入的封包大小
             * @param   packbuf[out]:拆解後傳出的封包內容
             * @return  封包去掉檔頭後的大小, 失敗為-1
             */
            virtual int readPacketHeader(Byte *data, int dataSize, Byte *packbuf) = 0;

            /**
             * 封包接收結束 先處理已接收封包
             * @param   command[in]:封包號碼
             * @param   data[in]:傳入的封包
             * @param   commandsize[in]:傳入的封包大小
             * @param   commandType[in]:封包類型
             * @return  回傳是否成功
             */
            virtual bool recvPack(int commandID, Byte *data, int commandSize, int CommandType) = 0;

        private:
            /**
             * @param   data[in,out]:要處理的buffer
             * @param   buffersize[in,out]:buffer現在的大小
             * @param   removeSize[in]:要移除的大小,ex.已parse的封包大小含檔頭
             */
            void adjustBuffer(Byte *data, int &bufferSize, int removeSize);

        private:
            /**
             * 連線位置
             */
            string _connectIP;

            /**
             * 連線埠號
             */
            int _connectPort;

        protected:
            /**
             * 當前正在接收的封包命令編號
             */
            int receivedCommandID;

            /**
             * 封包已接收的總長度
             */
            int receivedPacketLength;

            /**
             * 送出的封包原始長度
             */
            int sentPacketLength;

            /**
             * 送出的封包加密後長度
             */
            int sentCryptoPacketLength;

            /**
             * 封包加密器
             */
            Encrypter *_Encrypter;

        private:
            int _packIndex;

            bool _isPacketContinue;

            bool _isHeaderFinished;

            int _sockfd;

            Byte _RecvBuf[RECV_SOCKET_BUF_LEN];    //equal tom_iReceiveBufferLength

            int m_iReceiveBytes;    //目前recv buffer中未處理的大小

            bool m_bBlock;

        protected:
            union
            {
                struct
                {
                    UInt64  m_CRC           : 16;   // CRC
                    Int64   m_DummySize     : 3;    // For Triple DES encryption
                    UInt64  m_CommandID     : 13;   // Command ID
                    UInt64  m_CommandSize   : 22;   // Command size (Data)
                    UInt64  m_CommandType   : 2;    // COMMAND_TYPE
                    UInt64  m_Sequence      : 8;    // Sequence
                };
                UInt64  m_ui64Header;
            };
        };
    }
}

#endif //_SOCKETCLIENT_H_
