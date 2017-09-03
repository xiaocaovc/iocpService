/////////////////////////////////////////////////////////////////////////////
// Name:        IocpServer.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef IocpServer_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"IocpServerd.lib")
#else
#pragma comment(lib,"IocpServer.lib")
#endif
#define IocpServer_OP_API extern "C" _declspec(dllimport)
#endif

IocpServer_OP_API bool IocpSer_CreateServer(HANDLE hIocp, unsigned short nPort,
									void* OutDoMonitor,void* lpVoid=NULL,char* szSerIp=NULL);
IocpServer_OP_API bool IocpSer_Stop();