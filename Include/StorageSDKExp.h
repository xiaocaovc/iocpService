/////////////////////////////////////////////////////////////////////////////
// Name:        StorageSDK.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef StorageSDK_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"StorageSDKd.lib")
#else
#pragma comment(lib,"StorageSDK.lib")
#endif
#define StorageSDK_OP_API extern "C" _declspec(dllimport)
#endif
// 初始化
StorageSDK_OP_API bool ST_SDK_Init(void* pWnd,unsigned int LogMsg);
// 查询Camera
StorageSDK_OP_API int ST_SDK_SelectCam(void);
StorageSDK_OP_API bool ST_SDK_GetCamData(int nSock,int* CamId,int* Pid,char* CamName,short* CamType,
				char* CamIp,unsigned short* Port,char* User,char* Pass,int* Rot);
StorageSDK_OP_API bool ST_SDK_AddCam(int Pid,char* CamName,short CamType,char* CamIp,unsigned short nPort,char* User,char* Pass,int Rot);
// 编辑Camera
StorageSDK_OP_API bool ST_SDK_EditCam(int CamId,char* CamName,short CamType,char* CamIp,unsigned short nPort,char* User,char* Pass,int Rot);
// 删除Cam
StorageSDK_OP_API bool ST_SDK_DelCam(int CamId);
// 查询任务
StorageSDK_OP_API int ST_SDK_GetTaskStatus(int CamId);
// 开始任务
StorageSDK_OP_API bool ST_SDK_StartTask(int CamId);
// 停止任务
StorageSDK_OP_API bool ST_SDK_StopTask(int CamId);
// 播放录像
StorageSDK_OP_API void* ST_SDK_PlayRec(void* pWnd,int CamId, short CamType, LONGLONG sTime, LONGLONG eTime);
// 下载录像
StorageSDK_OP_API void* ST_SDK_DownLoadRec(void* pWnd,int CamId, LONGLONG sTime, LONGLONG eTime);
// 实时视频预览
StorageSDK_OP_API void* ST_SDK_PlayRealVideo(void* pWnd,int CamId, short CamType,
											 char* CamIp,unsigned short Port,char* User,char* Pass);
// 调整屏幕
StorageSDK_OP_API void ST_SDK_ResetScreen(void* pDecode);
// 停止播放
StorageSDK_OP_API void ST_SDK_StopPlay(void* pDecode);