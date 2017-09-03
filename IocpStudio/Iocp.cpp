#include "StdAfx.h"
#include "Iocp.h"

CIocp::CIocp(void)
:m_hIocp(NULL)
, m_nThreads(0)
,m_pThreads(NULL)
{
}

CIocp::~CIocp(void)
{
	if (m_hIocp)
	{
		CloseHandle(m_hIocp);
	}
	if (m_pThreads)
	{
		delete[] m_pThreads;
	}
}

// 停止完成端口
bool CIocp::Stop(void)
{
	if (!m_hIocp)
	{
		return true;
	}
	for(int i=0;i<m_nThreads;i++)
	{
		PostQueuedCompletionStatus(m_hIocp, 0, (DWORD)EXIT_CODE, NULL);
		Sleep(1);
	}
	 WaitForMultipleObjects(m_nThreads,m_pThreads, TRUE, INFINITE);
	return true;
}
// 初始化完成端口
void* CIocp::Init()
{
	if (m_hIocp)
	{
		return m_hIocp;
	}
	// 初始化IOCP
	// 建立第一个完成端口
	m_hIocp=CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if (NULL==m_hIocp)
	{
		/*CFileLog log;
		log	.WriteLog(1,_T("建立完成端口失败！错误代码: %d！"), WSAGetLastError());*/
		return NULL;
	}
	// 建立工作线程
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	m_nThreads=si.dwNumberOfProcessors*2+2;
	m_pThreads=new HANDLE[m_nThreads];
	DWORD nThreadID=0;
	for (int i=0;i<m_nThreads;i++)
	{
		m_pThreads[i]=CreateThread(0, 0, (LPTHREAD_START_ROUTINE)WorkerThread,this, 0, &nThreadID);
		// QueueUserWorkItem((LPTHREAD_START_ROUTINE)WorkerThread,this,WT_EXECUTEDEFAULT );
	}
	return m_hIocp;
}

// 绑定Iocp
bool CIocp::BindIocp(HANDLE hHandle,void* lpCompletionKey)
{
	if(!m_hIocp||NULL== CreateIoCompletionPort(hHandle,m_hIocp,(DWORD)lpCompletionKey, 0))
	{
		/*CFileLog log;
		log.WriteLog(1,"绑定完成端口失败！错误代码: %d！", WSAGetLastError());*/
		return false;
	}
	return true;
}
// 工作线程
DWORD /*WINAPI*/ CIocp::WorkerThread(CIocp* lpVoid)
{
	OVERLAPPED* lpOverlapped = NULL;
	COMPLETION_KEY_IOCP* lpCompletionKey = NULL;
	DWORD dwBytesTransfered = 0;
	BOOL bReturn=FALSE;
	while(TRUE)
	{
		bReturn = GetQueuedCompletionStatus(lpVoid->m_hIocp,
			&dwBytesTransfered,(PULONG_PTR)&lpCompletionKey,&lpOverlapped,
			INFINITE);
		// 如果收到的是退出标志，则直接退出
		if (EXIT_CODE==(DWORD)lpCompletionKey) // 线程退出
		{
			break;
		}
		else
		{
			// 内部回调函数
			lpCompletionKey->m_DoOutFun(bReturn,dwBytesTransfered,lpCompletionKey,lpOverlapped,lpCompletionKey->m_pVoid);
		}
	}
	return 0;
}

