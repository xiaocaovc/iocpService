/////////////////////////////////////////////////////////////////////////////
// Name:        iLog.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef iLog_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"iLogd.lib")
#else
#pragma comment(lib,"iLog.lib")
#endif
#define iLog_OP_API extern "C" _declspec(dllimport)
#endif

// 设置Log写入等级
iLog_OP_API int Log_SetLogLevel(int nLevel);
// 写入Log
iLog_OP_API void Log_WriteLog(int nLevel,char* Fmt,...);