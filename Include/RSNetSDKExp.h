/////////////////////////////////////////////////////////////////////////////
// Name:        RSNetSDK.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef RSNetSDK_OP_API
#else
#pragma comment(lib,"RSNetSDK.lib")
#define RSNetSDK_OP_API extern "C" _declspec(dllimport)
#endif

// 初始化
RSNetSDK_OP_API bool RS_NET_Init(void* hIocp,DWORD BufferSize);
// 实时视频
RSNetSDK_OP_API void* RS_NET_RealPlay(char* szSerIp,unsigned short nSerPort,void* DataBackFun,void* lpVoid);
// 释放
RSNetSDK_OP_API bool RS_NET_Release(void* lpCpKey);
// 停止
RSNetSDK_OP_API bool RS_NET_Stop();