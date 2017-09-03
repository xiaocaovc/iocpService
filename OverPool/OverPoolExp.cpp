/////////////////////////////////////////////////////////////////////////////
// Name:        OverPool.cpp
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#define OverPool_OP_API extern "C" _declspec(dllexport)
#include "OverPoolExp.h"
#include "OverBase.h"
COverBase Ov;
// 初始化
OverPool_OP_API bool Ov_Init(int BufferSize)
{
	return Ov.Init(BufferSize);
}
// 获得Ov结构
OverPool_OP_API void* Ov_GetOverlapped()
{
	return Ov.GetOver();
}
// 释放Ov结构
OverPool_OP_API void Ov_ReleaseOverlapped(void* pOverlapped)
{
	Ov.ReleaseOver((OVERLAPPED_PLUS*)pOverlapped);
}