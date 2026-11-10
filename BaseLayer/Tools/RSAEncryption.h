#ifndef _RSA_ENCRYPTION_H_
#define _RSA_ENCRYPTION_H_

#include "Encryption.h"
#include "Type.h"
#include <stddef.h>
#include <string.h>

//-----------------------------------------------------------------------------
// Class Definition : RSA¡¦s RC4 Encrypter
//-----------------------------------------------------------------------------
class RSAEncrypter : public Encrypter
{
public:
    RSAEncrypter()
    {
    }

    //copy constructor
    explicit RSAEncrypter(RSAEncrypter& rhs)
    {
        memcpy(m_SBox1, rhs.m_SBox1, sizeof(m_SBox1));
        memcpy(m_SBox2, rhs.m_SBox2, sizeof(m_SBox2));
    }

    virtual void SetKey(void* pchKey)
    {
        int i, j, keylen;
        Byte tmp;
        char *pkey = (char *)pchKey;

        for(i = 0 ; i < 256 ; i++)
            m_SBox1[i] = (Byte)i;

        j = 0;
        if(pkey)
        {
            keylen = (int)strlen(pkey);

            for(i = 0; i < 256 ; i++)
            {
                if(j == keylen)
                    j = 0;

                m_SBox2[i] = pkey[j++];
            }
        }
        else
        {
            for(i = 0; i < 256 ; i++)
            {
                m_SBox2[i] = (Byte)((i * 7 + 13) & 0xff);
            }
        }

        // Scramble m_SBox1 with m_SBox2
        j = 0;

        for(i = 0; i < 256; i++)
        {
            j = (j + (Dword)m_SBox1[i] + (Dword)m_SBox2[i]) % 256;
            tmp = m_SBox1[i];
            m_SBox1[i] = m_SBox1[j];
            m_SBox1[j] = tmp;
        }
    }

    virtual char *Decrypt(char *pchBuf, int iLen)
    {
        int i, j;

        if(pchBuf != NULL)
        {
            for(i = 0; i < iLen; i++)
            {
                j = (i + 7) & 0xff;
                pchBuf[i] = (pchBuf[i] ^ m_SBox1[j]);
            }
        }

        return pchBuf;
    }

    virtual char *Encrypt(char *pchBuf, int iLen) {
        int i, j;

        if(pchBuf != NULL)
        {
            for(i = 0; i < iLen; i++) {
                j					= (i + 7) & 0xff;
                pchBuf[i] = (pchBuf[i] ^ m_SBox1[j]);
            }
        }

        return pchBuf;
    }

    virtual Encrypter* Clone() {
        return new RSAEncrypter(*this);
    }

    virtual int Alignment(int iLen) {
        return iLen;
    }

private:

    Byte m_SBox1[256];
    Byte m_SBox2[256];

};

#endif //_RSA_ENCRYPTION_H_
