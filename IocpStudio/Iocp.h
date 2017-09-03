#pragma once
/*************************说明*******************************
1.根据CPU核心确定线程数量(定线程数量)
2.采用回调的方式扩展处理接口
3.Key结构基础部分必须相同,Over结构依然
*************************说明*******************************/
// 传递给Worker线程的退出信号
#define EXIT_CODE NULL
typedef void (*DoFun)(BOOL bRes,DWORD dwBytesTransfered,
					  void* lpCompletionKey,void* lpOverlapped,void* pVoid);
typedef struct _COMPLETION_KEY_IOCP
{
	// -----外部会调用-----
	DoFun m_DoOutFun; // 外部处理函数
	void* m_pVoid;    // 外回调对象
	void* m_pObj;     // 交互对象

	_COMPLETION_KEY_IOCP* pLink;
	_COMPLETION_KEY_IOCP* pNext;
}COMPLETION_KEY_IOCP;

class CIocp
{
public:
	CIocp(void);
	~CIocp(void);
private:
	// 完成端口,每个服务器只有一个
	HANDLE m_hIocp;
	// 线程个数
	int m_nThreads;
	HANDLE* m_pThreads;
private:
	// 工作线程
	static DWORD /*WINAPI*/ WorkerThread(CIocp* lpVoid);
	// 绑定Iocp
	bool BindIocp(HANDLE hHandle,void* lpCompletionKey);
public:
	// 初始化完成端口,线程数量,0为默认线程数
	void* Init();
	// 停止完成端口
	bool Stop(void);
};
