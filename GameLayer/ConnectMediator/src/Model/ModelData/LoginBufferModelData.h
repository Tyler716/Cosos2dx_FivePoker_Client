#ifndef _LOGINBUFFERMODELDATA_H_
#define _LOGINBUFFERMODELDATA_H_

#include "IData.h"
#include <wchar.h>

using BaseLayer::Common::IData;

struct LoginBufferModelData : public IData
{
    LoginBufferModelData(const wchar_t *account = NULL, const wchar_t *password = NULL)
    {
        if(account != NULL)
        {
#if defined(_WIN32) || defined(_WIN64)
            wcscpy_s(m_szAccount, MAX_ACCOUNT, account);
#else
            wcsncpy( m_szAccount, account, MAX_ACCOUNT);
#endif  //defined(_WIN32) || defined(_WIN64)
        }

        if(password != NULL)
        {
#if defined(_WIN32) || defined(_WIN64)
            wcscpy_s(m_szPassword, MAX_PASSWORD, password);
#else
            wcsncpy( m_szPassword, password, MAX_PASSWORD);
#endif  //defined(_WIN32) || defined(_WIN64)
        }

        centralServerLoginKey = 0;
    }

    bool isLegalRequest()
    {
        return (wcslen(m_szAccount) != 0 && wcslen(m_szPassword) != 0);
    }

    wchar_t m_szAccount[MAX_ACCOUNT];
    wchar_t m_szPassword[MAX_ACCOUNT];

    int centralServerLoginKey;
};

#endif //_LOGINBUFFERMODELDATA_H_
