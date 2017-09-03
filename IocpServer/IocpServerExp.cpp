/////////////////////////////////////////////////////////////////////////////
// Name:        IocpServer.cpp
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#define IocpServer_OP_API extern "C" _declspec(dllexport)
#include "IocpServerExp.h"
#include "TcpServer.h"
//#include "KeyPool.h"
//CKeyPool<_COMPLETION_KEY_NET> g_KeyList;
CTcpServer tcpServer;

IocpServer_OP_API bool IocpSer_CreateServer(HANDLE hIocp, unsigned short nPort,
									void* OutDoMonitor,void* lpVoid,char* szSerIp)
{
	return tcpServer.CreateServer(hIocp,nPort,OutDoMonitor,lpVoid,szSerIp);
}
IocpServer_OP_API bool IocpSer_Stop()
{
	return tcpServer.Stop();
}