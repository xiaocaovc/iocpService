#ifndef _COMMON_STRUCT_IO_
#define _COMMON_STRUCT_IO_

typedef enum _OPERATION_TYPE  
{
	NULL_POSTED,    // 用于初始化，无意义
	ACCEPT_POSTED, // 标志投递的Accept操作
	SEND_POSTED,   // 标志投递的是发送操作
	RECV_POSTED,   // 标志投递的是接收操作
	WRITE_POSTED,  // 标志投递的是写入操作
	READ_POSTED,   // 标志投递的是读取操作

}OPERATION_TYPE;
typedef void (*DoFun)(BOOL bRes,DWORD dwBytesTransfered,
					  void* lpCompletionKey,void* lpOverlapped,void* pVoid);
typedef struct _OVERLAPPEDEX
{
	OVERLAPPED      m_Overlapped; // 重叠结构
	OPERATION_TYPE  m_OpType;     // 操作标识
	char*           m_pBuffer;    // 数据
	// ---------管理用数据-----------
	_OVERLAPPEDEX* pLink;
	_OVERLAPPEDEX* pNext;
}OVERLAPPEDEX;
typedef struct _COMPLETION_KEY
{
	// -----外部会调用-----
	DoFun m_DoOutFun; // 外部处理函数
	void* m_pVoid;    // 外回调对象
	void* m_pObj;     // 交互对象
	// ---------管理用数据-----------
	_COMPLETION_KEY* pLink;
	_COMPLETION_KEY* pNext;
}COMPLETION_KEY;
#endif