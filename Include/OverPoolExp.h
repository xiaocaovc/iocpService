/////////////////////////////////////////////////////////////////////////////
// Name:        OverPool.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef OverPool_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"OverPoold.lib")
#else
#pragma comment(lib,"OverPool.lib")
#endif
#define OverPool_OP_API extern "C" _declspec(dllimport)
#endif
//typedef struct OVERLAPPEDEX
//{
//	OVERLAPPED      m_Overlapped; // 重叠结构
//	int  m_OpType;     // 操作标识
//	char*           m_pBuffer;    // 数据
//};
// 初始化
OverPool_OP_API bool Ov_Init(int BufferSize=102400);
// 获得Ov结构
OverPool_OP_API void* Ov_GetOverlapped();
// 释放Ov结构
OverPool_OP_API void Ov_ReleaseOverlapped(void* pOverlapped);