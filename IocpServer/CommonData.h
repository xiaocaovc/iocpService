#ifndef _COMMON_DATA_IO_
#define _COMMON_DATA_IO_
#include "Common.h"
typedef struct _OVERLAPPED_PLUS:public OVERLAPPEDEX
{
	DWORD			m_BufferSize; // 数据大小
	_OVERLAPPED_PLUS()
	{
		m_BufferSize=0;
	}
}OVERLAPPED_PLUS;
typedef struct _COMPLETION_KEY_NET:public COMPLETION_KEY
{
	DoFun m_DoInFun;  // 内回调函数(第一个)
	SOCKET m_Socket;
	// 接收数据
	unsigned long m_nLeftBytes;
	unsigned long m_nDataLen;
}COMPLETION_KEY_NET;
#endif
