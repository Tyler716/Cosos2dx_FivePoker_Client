#ifndef _TYPE_H_
#define _TYPE_H_

#define S16                                         16
#define S32                                         32

// 遊戲的基本型別定義
typedef char				Char;
typedef unsigned char		Byte;
typedef unsigned char		UChar;
typedef short				Short;
typedef unsigned short		Word;
typedef unsigned short		UShort;
typedef int					Int;
typedef unsigned int		Dword;
typedef unsigned int		UInt;
typedef long long			Int64;
typedef unsigned long long	UInt64;
typedef char*               PChar;

typedef wchar_t             STR16[S16];        ///< char pchBuf[16]

#if defined(WIN32) && defined(UNICODE)
typedef wchar_t				TChar;
#else
typedef char				TChar;
#endif

//=============================================================================
// NULL definition
#ifndef NULL
#define NULL	0
#endif
#ifndef Null
#define Null	0
#endif

#define	ARRAY_SIZE(array)			(sizeof((array)) / sizeof((array)[0]))

#endif //_TYPE_H_
