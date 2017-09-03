/////////////////////////////////////////////////////////////////////////////
// Name:        iLog.cpp
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#define iLog_OP_API extern "C" _declspec(dllexport)
#include "iLogExp.h"
#include "FileLog.h"

#define  LOG_BUF_SIZE 1024
CFileLog Log;
iLog_OP_API int Log_SetLogLevel(int nLevel)
{
	return Log.SetLogLevel(nLevel);
}
iLog_OP_API void Log_WriteLog(int nLevel,char* Fmt,...)
{
	if (Log.GetLogLevel(nLevel))
	{
		return;
	}
	char sztBuf[LOG_BUF_SIZE];
	time_t curTime;
	struct tm mt;
	curTime = time(NULL);
	try
	{
		localtime_s(&mt,&curTime);
		strftime(sztBuf,log_buf_size,"%Y-%m-%d %H:%M:%S",&mt);
		int nLen=strlen(sztBuf);
		sprintf_s(sztBuf+nLen,log_buf_size-nLen," LV:%d DESC:",nLevel);
		nLen=strlen(sztBuf);
		va_list argptr;          //分析字符串的格式
		va_start(argptr, Fmt);
		_vsnprintf_s(sztBuf+nLen, LOG_BUF_SIZE-nLen,_TRUNCATE, Fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		return;
	}
	Log.WriteLog(sztBuf,&mt);
}