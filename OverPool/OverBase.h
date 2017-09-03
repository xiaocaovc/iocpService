#pragma once

typedef enum _OPERATION_TYPE  
{
	NULL_POSTED,    // 用于初始化，无意义
	ACCEPT_POSTED, // 标志投递的Accept操作
	SEND_POSTED,   // 标志投递的是发送操作
	RECV_POSTED,   // 标志投递的是接收操作
	WRITE_POSTED,  // 标志投递的是写入操作
	READ_POSTED,   // 标志投递的是读取操作

}OPERATION_TYPE;

typedef struct _OVERLAPPED_PLUS
{
	OVERLAPPED      m_Overlapped; // 重叠结构
	OPERATION_TYPE  m_OpType;     // 操作标识
	char*           m_pBuffer;    // 数据

	DWORD			m_BufferSize; // 数据大小
	DWORD			m_dwFlags;
	DWORD			m_dwBytes;
	WSABUF			m_wsaBuf[1];

	_OVERLAPPED_PLUS* pLink;
	_OVERLAPPED_PLUS* pNext;
	_OVERLAPPED_PLUS()
	{
		m_OpType=NULL_POSTED;
		ZeroMemory(&m_Overlapped,sizeof(OVERLAPPED));
		m_pBuffer=NULL;
		m_BufferSize=0;
		m_dwFlags=0;
		m_dwBytes=0;
	}
	~_OVERLAPPED_PLUS()
	{
		if (m_pBuffer)
		{
			delete[] m_pBuffer;
		}
	}
}OVERLAPPED_PLUS;

class COverBase
{
public:
	COverBase(void);
	~COverBase(void);
private:
	CRITICAL_SECTION m_cs;
	// 数据大小
	int m_nSize;
	// 空闲数据头指针
	OVERLAPPED_PLUS* m_pIdle;
	// 数据链头
	OVERLAPPED_PLUS* m_pLink;
public:
	// 初始化
	bool Init(int nSize);
	// 获取空闲数据
	void* GetOver();
	// 释放Over
	void ReleaseOver(OVERLAPPED_PLUS* pOver);
};
