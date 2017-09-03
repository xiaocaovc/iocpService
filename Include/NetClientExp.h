/////////////////////////////////////////////////////////////////////////////
// Name:        NetClient.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef NetClient_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"NetClientd.lib")
#else
#pragma comment(lib,"NetClient.lib")
#endif
#define NetClient_OP_API extern "C" _declspec(dllimport)
#endif

// 初始化客户端返回lpCompletionKey
NetClient_OP_API void* NetCli_Init(void* hIocp,char* szSerIp, unsigned short nSerPort,int nMaxPending,  
								   void* DoOutFun=NULL,void* pVoid=NULL);
// 发送消息 返回值:0 正确,-1,锁定,1错误,2丢包
NetClient_OP_API int NetCli_PostSend(void* lpCompletionKey,void* pOverlapped,int Cmd,DWORD dwBytesLen);
// 关闭连接,是否等待发送完成(包括锁定的)
NetClient_OP_API bool NetCli_Release(void* lpCompletionKey,bool bCompleted=false);
// 停止客户端
NetClient_OP_API bool NetCli_Stop();