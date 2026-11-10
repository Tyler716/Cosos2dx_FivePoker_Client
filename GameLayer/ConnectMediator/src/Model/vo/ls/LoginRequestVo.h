#ifndef _LOGINREQUESTVO_H
#define _LOGINREQUESTVO_H

#include "Common.h"
#include <wchar.h>

struct LoginRequestVo
{
    LoginRequestVo(const wchar_t *account, const wchar_t *password)
    {
        if(account != NULL)
        {
#if defined(_WIN32) || defined(_WIN64)
            wcscpy_s( m_szAccount, MAX_ACCOUNT, account);
#else
            wcsncpy(m_szAccount, account, MAX_ACCOUNT);
#endif  //defined(_WIN32) || defined(_WIN64)
        }

        if(password != NULL)
        {
#if defined(_WIN32) || defined(_WIN64)
            wcscpy_s( m_szPassword, MAX_PASSWORD, password);
#else
            wcsncpy(m_szPassword, password, MAX_PASSWORD);
#endif  //defined(_WIN32) || defined(_WIN64)
        }
    }

    bool isLegalRequest()
    {
        return (wcslen(m_szAccount) != 0 && wcslen(m_szPassword) != 0);
    }

    wchar_t m_szAccount[MAX_ACCOUNT];
    wchar_t m_szPassword[MAX_PASSWORD];
};

#endif //_LOGINREQUESTVO_H
