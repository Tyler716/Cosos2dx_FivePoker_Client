#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_

//-----------------------------------------------------------------------------
// Class Definition : Encrypter
//-----------------------------------------------------------------------------
class Encrypter
{
public:
	//-------------------------------------------------------------------------
	// Public Method : Operations
	//-------------------------------------------------------------------------

	// Cary Added in 05/01/19
	// 設定加密的Key
	// end
	virtual void SetKey( void* key ) {}

	// Cary Added in 05/01/19
	// 解密
	// end
	virtual char* Decrypt( char* buf, int len ) { return buf; }

	// Cary Added in 05/01/19
	// 加密
	// end
	virtual char* Encrypt( char* buf, int len ) { return buf; }

	// Cary Added in 05/01/19
	// 複製出同一類別的物件
	// end
	virtual Encrypter* Clone() {
		return new Encrypter;
	}

    // Cary modified at 2005/04/24
    // 調整複製資料所需的長度
    virtual int Alignment( int len ) { return len; }
    //end

};

#endif  //_ENCRYPTION_H_
