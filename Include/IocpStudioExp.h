/////////////////////////////////////////////////////////////////////////////
// Name:        IocpStudio.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef IocpStudio_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"IocpStudiod.lib")
#else
#pragma comment(lib,"IocpStudio.lib")
#endif

#define IocpStudio_OP_API extern "C" _declspec(dllimport)
#endif

// 初始化IOCP,返回HANDLE of IOCP
IocpStudio_OP_API void* Iocp_Init();
// 停止,要在释放资源前停止
IocpStudio_OP_API bool Iocp_Stop();