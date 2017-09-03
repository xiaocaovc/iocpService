/////////////////////////////////////////////////////////////////////////////
// Name:        IocpStudio.cpp
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#define IocpStudio_OP_API extern "C" _declspec(dllexport)
#include "IocpStudioExp.h"
#include "Iocp.h"
CIocp Iocp;
// 初始化IOCP,返回HANDLE of IOCP
IocpStudio_OP_API void* Iocp_Init()
{
	return Iocp.Init();
}
// 停止,要在释放资源前停止
IocpStudio_OP_API bool Iocp_Stop()
{
	return Iocp.Stop();
}