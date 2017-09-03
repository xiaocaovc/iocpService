/****************************说明****************************
负责建立服务端,完成实际的Key结构
****************************说明****************************/
#pragma once
#include "CommonData.h"
#include <winsock2.h>
#include <MSWSock.h>
#pragma comment(lib,"ws2_32.lib")

class CTcpServer:public COMPLETION_KEY
{
public:
	CTcpServer(void)
		:m_Socket(INVALID_SOCKET)
		,m_OverAccept(NULL)
	{
		m_DoOutFun=(DoFun)DoInMonitor;
		WSADATA wsaData;
		// 错误(一般都不可能出现)
		if (NO_ERROR!=WSAStartup(MAKEWORD(2,2), &wsaData))
		{
			TRACE("---初始化WinSock 2.2失败!---\n");
		}
	}
	~CTcpServer(void)
	{
		if (INVALID_SOCKET!=m_Socket)
		{
			closesocket(m_Socket);
			m_Socket=INVALID_SOCKET;
		}
		if (m_OverAccept)
		{
			delete[] m_OverAccept;
		}
		WSACleanup();
	}
private:
	// Accept结构
	typedef struct _OVERLAPPED_ACCEPT
	{
		OVERLAPPED   m_Overlapped; // 重叠结构 m_Overlapped;
		SOCKET m_Socket;
		char m_Buff[(sizeof(SOCKADDR_IN)+16)*2];
		_OVERLAPPED_ACCEPT()
		{
			ZeroMemory(&m_Overlapped,sizeof(OVERLAPPED));
			m_Socket=INVALID_SOCKET;
		}
		~_OVERLAPPED_ACCEPT()
		{
			if (INVALID_SOCKET!=m_Socket)
			{
				LINGER lingerStruct;
				lingerStruct.l_onoff = 1;
				lingerStruct.l_linger = 0;
				setsockopt(m_Socket, SOL_SOCKET, SO_LINGER,
					(char *)&lingerStruct, sizeof(lingerStruct) );
				CancelIo((HANDLE)m_Socket);
				closesocket(m_Socket);
				m_Socket=INVALID_SOCKET;
			}
		}
	}OVERLAPPED_ACCEPT;

	bool PostAccept(OVERLAPPED_ACCEPT* lpOverlapped)
	{
		ASSERT( INVALID_SOCKET!=m_Socket );
		// 准备参数
		DWORD dwBytes = 0;  
		// 为以后新连入的客户端先准备好Socket( 这个是与传统accept最大的区别 ) 
		lpOverlapped->m_Socket= WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  
		if(INVALID_SOCKET==lpOverlapped->m_Socket) 
		{  
			TRACE("-----创建用于Accept的Socket失败！错误代码: %d-----\n", WSAGetLastError()); 
			return false;  
		} 
		// 投递AcceptEx
		ZeroMemory(&lpOverlapped->m_Overlapped,sizeof(lpOverlapped->m_Overlapped));
		if(!m_lpfnAcceptEx(m_Socket, lpOverlapped->m_Socket,
			&lpOverlapped->m_Buff, 0/*p_wbuf->len - ((sizeof(SOCKADDR_IN)+16)*2*/,
			sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwBytes,& lpOverlapped->m_Overlapped))
		{  
			if(WSA_IO_PENDING!=WSAGetLastError())  
			{  
				TRACE("-----投递 AcceptEx 请求失败，错误代码: %d-----\n", WSAGetLastError());
				return false;  
			}  
		} 
		return true;
	}
	// 内部数据
private:
	DoFun m_DoFun;  // 内回调函数
	// 监听线程
	SOCKET m_Socket;
	// IOCP句柄
	HANDLE m_hIocp;
	LPFN_ACCEPTEX m_lpfnAcceptEx;
	//#ifdef _DEBUG
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockAddrs;
	OVERLAPPED_ACCEPT* m_OverAccept;

private:
	// 监视回调
	static void DoInMonitor(BOOL bRes,unsigned long dwBytesTransfered,CTcpServer* lpCompletionKey,
		OVERLAPPED_ACCEPT* lpOverlapped,void* lpVoid)
	{
		if (!bRes)
		{
			TRACE(_T("-----监视连入发生错误或监视退出!-----\n"));
			return;
		}
		char szIp[32];
		//#ifdef _DEBUG
			//OVERLAPPED_ACCEPT* pAccept=CONTAINING_RECORD(lpOverlapped,OVERLAPPED_ACCEPT,m_Overlapped);
		SOCKADDR_IN* ClientAddr = NULL;
		SOCKADDR_IN* LocalAddr = NULL;  
		int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);  
		lpCompletionKey->m_lpfnGetAcceptExSockAddrs(lpOverlapped->m_Buff, 0,  
			sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);
		TRACE(_T("-----客户端 %s:%d 连入,时间:%u.-----\n"), inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port),clock());
		sprintf_s(szIp,32,"IP:%s,Port:%d",inet_ntoa(ClientAddr->sin_addr),ntohs(ClientAddr->sin_port));
		//#endif
		SOCKET nSocket=lpOverlapped->m_Socket;
		// 继续投递AcceptEx接受客户端连接
		lpCompletionKey->PostAccept(lpOverlapped);
		// 监视回调
		lpCompletionKey->m_DoFun(bRes,dwBytesTransfered,(void*)nSocket,szIp,lpVoid);
	}
public:
	// 生成服务端
	bool CreateServer(HANDLE hIocp, unsigned short nPort,
		void* OutDoMonitor,void* pVoid=NULL,char* szSerIp=NULL)
	{
		m_hIocp=hIocp;
		m_DoFun=(DoFun)OutDoMonitor;// 监视连接
		m_pVoid=pVoid;
		// 建立监视Socket
		m_Socket= WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET==m_Socket) 
		{
			TRACE(_T("初始化监视Socket失败，错误代码: %d."), WSAGetLastError());
			return false;
		}
		// 服务器地址信息，用于绑定Socket
		struct sockaddr_in ServerAddress;
		// 填充地址信息
		ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
		ServerAddress.sin_family = AF_INET;
		// 这里可以绑定任何可用的IP地址，或者绑定一个指定的IP地址
		if (szSerIp)
		{
			ServerAddress.sin_addr.s_addr = inet_addr(szSerIp);  
		}
		else
		{
			ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		ServerAddress.sin_port = htons(nPort);
		// 绑定地址和端口
		if (SOCKET_ERROR == bind(m_Socket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))) 
		{
			TRACE("bind()函数执行错误！");
			return false;
		}
		// 开始进行监听
		if (SOCKET_ERROR==listen(m_Socket,SOMAXCONN))
		{
			TRACE("监听失败！错误代码: %d.", WSAGetLastError());
			return false;
		}

		// AcceptEx 和 GetAcceptExSockaddrs 的GUID，用于导出函数指针
		GUID GuidAcceptEx = WSAID_ACCEPTEX;  
		GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
		// 获取AcceptEx函数指针
		DWORD dwBytes = 0;  
		if(SOCKET_ERROR == WSAIoctl(m_Socket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidAcceptEx, 
			sizeof(GuidAcceptEx), 
			&m_lpfnAcceptEx, 
			sizeof(LPFN_ACCEPTEX), 
			&dwBytes, 
			NULL, 
			NULL))  
		{  
			TRACE("WSAIoctl 未能获取AcceptEx函数指针。错误代码: %d!!", WSAGetLastError());
			return false;  
		}
		//#ifdef _DEBUG
		// 获取GetAcceptExSockAddrs函数指针，也是同理
		if(SOCKET_ERROR == WSAIoctl(m_Socket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidGetAcceptExSockAddrs,
			sizeof(GuidGetAcceptExSockAddrs), 
			&m_lpfnGetAcceptExSockAddrs, 
			sizeof(m_lpfnGetAcceptExSockAddrs),   
			&dwBytes, 
			NULL, 
			NULL))  
		{
			TRACE("WSAIoctl 未能获取GuidGetAcceptExSockAddrs函数指针。错误代码: %d!!", WSAGetLastError());
			return false; 
		}
		//#endif
		// 投递AcceptEx I/O请求
		// 绑定完成端口
		if( NULL== CreateIoCompletionPort((HANDLE)m_Socket, m_hIocp,
			(DWORD)(this), 0))  
		{
			TRACE(_T("绑定 Listen Socket至完成端口失败！错误代码: %d！"), WSAGetLastError());
			return false;
		}
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		unsigned int nAccept=si.dwNumberOfProcessors*2+2;
		m_OverAccept=new OVERLAPPED_ACCEPT[nAccept];
		for(unsigned int i=0;i<nAccept;i++)
		{
			// 新建一个IO_CONTEXT
			if(!PostAccept(&m_OverAccept[i]))
			{
				TRACE(_T("PostAccept建立监听失败！错误代码: %d！"), WSAGetLastError());
			}
		}
		return true;
	}
	bool Stop()
	{
		if (INVALID_SOCKET!=m_Socket)
		{
			closesocket(m_Socket);
			m_Socket=INVALID_SOCKET;
			return true;
		}
		return false;
	}
};
