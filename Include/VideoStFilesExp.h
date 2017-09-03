/////////////////////////////////////////////////////////////////////////////
// Name:        VideoStFiles.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef VideoStFiles_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"VideoStFilesd.lib")
#else
#pragma comment(lib,"VideoStFiles.lib")
#endif

#define VideoStFiles_OP_API extern "C" _declspec(dllimport)
#endif
// -----------------初始化模块------------------------------

// 初始化模块,完成端口句柄,存储路径,Buff大小,交互函数
VideoStFiles_OP_API bool VstFile_Init(void* hIocp,int StorDay=30);

// ----------------写操作------------------------------------

// 生成存储文件
VideoStFiles_OP_API bool VstFile_CreateStorage(int CamId, int nIdx, int nDat);

// 初始化写,返回lpCompletionKey,hFile
VideoStFiles_OP_API void* VstFile_InitWrite(int CamId,void* DoOutFun=NULL,void* pVoid=NULL,bool bCompose=false);

// 写数据
VideoStFiles_OP_API bool VstFile_WriteData(void* lpCompletionKey,void*lpOverlapped);

// 写头文件
VideoStFiles_OP_API bool VstFile_WriteHead(void* lpCompletionKey,char* pBuffer, int dwBytesLen);

// 获得CamId
VideoStFiles_OP_API int VstFile_GetIoCamId(void* lpCompletionKey);

// ------------------读取操作---------------------------------

// 初始化读,返回lpCompletionKey,hFile
VideoStFiles_OP_API void* VstFile_InitRead(int CamId,LONGLONG sTime,LONGLONG eTime,
										   void* DoOutFun=NULL,void* pVoid=NULL);

// 读数据
VideoStFiles_OP_API bool VstFile_ReadData(void* lpCompletionKey,void*lpOverlapped);

// 读头
VideoStFiles_OP_API bool VstFile_ReadHead(char* pBuffer, int* dwBytesLen, int CamId);

VideoStFiles_OP_API int  VstFile_GetCamId(void* lpCompletionKey);

// ------------------释放资源--------------------------------


// 释放资源
VideoStFiles_OP_API bool VstFile_Release(void* lpCompletionKey);

// 停用模块,释放资源
VideoStFiles_OP_API bool VstFile_Stop();