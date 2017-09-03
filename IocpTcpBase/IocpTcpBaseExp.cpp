/////////////////////////////////////////////////////////////////////////////
// Name:        IocpTcpBase.cpp
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#define IocpTcpBase_OP_API extern "C" _declspec(dllexport)
#include "IocpTcpBaseExp.h"
// 绑定IOCP
IocpTcpBase_OP_API bool IocpTcp_BindIocp(unsigned int nSocket,void* hIocp,void* lpCompletionKey)
{
	if (NULL==CreateIoCompletionPort((HANDLE)nSocket,
		hIocp, (DWORD)lpCompletionKey, 0))
	{
		TRACE("-----客户SOCKET绑定IOCP出现错误.错误代码:%d-----",GetLastError());
		return false;
	}
	return true;
}
// 投递接收
IocpTcpBase_OP_API bool IocpTcp_PostRecv(unsigned int nSocket,void* lpOverlapped,char* pBuffer,DWORD dwBytesLen)
{
	ASSERT(INVALID_SOCKET!=nSocket);
	DWORD dwFlags=0,dwBytes=0;
	WSABUF wsaBuf[1];
	wsaBuf[0].len=dwBytesLen;
	wsaBuf[0].buf=pBuffer;
	// 初始化
	((OVERLAPPED*)lpOverlapped)->hEvent=NULL;
	// 投递WSARecv请求
	//ZeroMemory(lpOverlapped->m_Overlapped,sizeof(OVERLAPPED));
	if (SOCKET_ERROR==WSARecv((SOCKET)nSocket, wsaBuf, 1,&dwBytes,&dwFlags, (OVERLAPPED*)lpOverlapped, NULL ))
	{
		if(WSA_IO_PENDING!=WSAGetLastError())  
		{
			TRACE("---服务端接收错误SOCKET:%d,释放资源,错误代码:%d---\n",nSocket,WSAGetLastError());
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}
// 投递发送
IocpTcpBase_OP_API bool IocpTcp_PostSend(unsigned int nSocket,void* lpOverlapped,char* pBuffer,DWORD dwBytesLen)
{
	DWORD dwFlags=0,dwBytes=0;
	WSABUF wsaBuf[1];
	wsaBuf[0].len=dwBytesLen;
	wsaBuf[0].buf=pBuffer;
	// 初始化
	((OVERLAPPED*)lpOverlapped)->hEvent=NULL;
	// 投递发送
	//ZeroMemory(&lpOverlapped->m_Overlapped,sizeof(OVERLAPPED));
	if (SOCKET_ERROR==WSASend((SOCKET)nSocket,wsaBuf, 1,&dwBytes,dwFlags,(OVERLAPPED*)lpOverlapped, NULL))
	{
		if(WSA_IO_PENDING!=WSAGetLastError())  
		{
			TRACE("---_COMPLETION_KEY_NET::PostSend 服务端发送失败SOCKET:%d,释放资源,错误代码:%d!---\n",nSocket,WSAGetLastError());
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}
// 停止
IocpTcpBase_OP_API bool IocpTcp_StopSock(unsigned int nSocket,bool bCompleted)
{
	LINGER lingerStruct;
	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = bCompleted?10:0;
	setsockopt((SOCKET)nSocket, SOL_SOCKET, SO_LINGER,
		(char *)&lingerStruct, sizeof(lingerStruct) );
	//CancelIo((HANDLE)nSocket);
	return 0==closesocket((SOCKET)nSocket);
}