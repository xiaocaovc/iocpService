/////////////////////////////////////////////////////////////////////////////
// Name:        NetServer.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef NetServer_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"NetServerd.lib")
#else
#pragma comment(lib,"NetServer.lib")
#endif
#define NetServer_OP_API extern "C" _declspec(dllimport)
#endif
// 初始化服务器 INVALID_SOCKET
NetServer_OP_API bool NetSvr_Init(HANDLE hIocp, unsigned short nPort,
								  void* OutDoFun=NULL,void* pObj=NULL,char* szSerIp=NULL);
// 服务端发送 1,错误,0,-1 缓冲,2丢包
NetServer_OP_API int NetSvr_PostSend(void* lpCompletionKey,void* pOverlapped,int Cmd,DWORD dwBytesLen);
// 关闭连接,bCompleted决定是否完成所有发送(包括Pending),否则立即结束
NetServer_OP_API bool NetSvr_Release(void* lpCompletionKey,bool bCompleted=false);

// 停止服务端
NetServer_OP_API bool NetSvr_Stop();