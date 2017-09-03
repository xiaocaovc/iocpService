/////////////////////////////////////////////////////////////////////////////
// Name:        IocpTcpBase.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef IocpTcpBase_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"IocpTcpBased.lib")
#else
#pragma comment(lib,"IocpTcpBase.lib")
#endif
#define IocpTcpBase_OP_API extern "C" _declspec(dllimport)
#endif
// 绑定IOCP
IocpTcpBase_OP_API bool IocpTcp_BindIocp(unsigned int nSocket,void* hIocp,void* lpCompletionKey);
// 投递接收
IocpTcpBase_OP_API bool IocpTcp_PostRecv(unsigned int nSocket,void* lpOverlapped,char* pBuffer,DWORD dwBytesLen);
// 投递发送
IocpTcpBase_OP_API bool IocpTcp_PostSend(unsigned int nSocket,void* lpOverlapped,char* pBuffer,DWORD dwBytesLen);
// 停止
IocpTcpBase_OP_API bool IocpTcp_StopSock(unsigned int nSocket,bool bCompleted=false);