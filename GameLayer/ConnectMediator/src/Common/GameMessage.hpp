#ifndef _GAMEMESSAGE_HPP_
#define _GAMEMESSAGE_HPP_

#include <queue>     // STL header file for queue
#include <list>

#if !defined(_WIN32) && !defined(_WIN64)
#include <cstddef>
#endif //!defined(_WIN32) && !defined(_WIN64)

using namespace std; // Specify that we are using the std namespace

struct Message
{
    Message(int iCmdID, const void *pData)
    {
        m_iCmdID = iCmdID;
        m_pData = pData;
    }
    
    int     m_iCmdID;      ///< 訊息ID
    const void*   m_pData;      ///< 訊息資料
};


class Message_Queue
{
    typedef queue<Message *, list<Message *> > MsgQueType;

    MsgQueType m_msgQueue;
      
public:
    void Add(Message *pMsg)
    {
        // Insert the element at the end of the queue
        m_msgQueue.push(pMsg);
    }

    Message *Remove()
    {
        Message *pMsg = NULL;

        if(bLock)
        {
            return NULL;
        }
      
        // Check if the message queue is not empty
        if (!m_msgQueue.empty())
        {
             // Queue is not empty so get a pointer to the
             // first message in the queue
             pMsg = m_msgQueue.front();
         
             // Now remove the pointer from the message queue
             m_msgQueue.pop();
        }
        return pMsg;
    }
   
    int GetLength() const
    {
        return m_msgQueue.size();
    }

    void SetLock()
    {
        bLock = true;
    }

    void SetUnLock()
    {
        bLock = false;
    }

    bool GetLock()
    {
        return bLock;
    }

private:
    bool bLock;
};

#endif  //_GAMEMESSAGE_HPP_