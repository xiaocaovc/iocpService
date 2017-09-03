/*********************************说明*********************************
1.支持多线程 1~100
2.效率(1百万)
 A.单线程:13853ms:72.18657pcs/ms
 B:10线程:6661ms:150.1276pcs/ms,
 C.100线程:7301ms:136.9675pcs/ms
3.使用方法
 可以使用全局对象
4.编程细节
 文件常开,自动推送,串行推入
*********************************说明*********************************/
#pragma once
#include <time.h>
#include <share.h>
#include  <io.h>
#include <sys/stat.h>
//#include <fstream>
#define log_buf_size 1024

class CFileLog
{
public:
	CFileLog(void)
	: m_nPath(0)
	, m_CurDay(0)
	, m_nLogLevel(2)
	,pfp(NULL)
	{
		// 初始化临界区
		InitializeCriticalSection(&m_cs);
		GetModuleFileNameA(GetSelfModuleHandle(), m_szPath, MAX_PATH );
		int nLen=strlen(m_szPath);
		for (int i=nLen;i>0;i--) //去模块名
		{
			if (m_szPath[i]=='\\'||m_szPath[i]=='/')
			{
				strcpy_s(m_szPath+i+1,MAX_PATH-i-1,"Log\\");// 添加Log文件夹
				break;
			}
		}
		m_nPath=strlen(m_szPath);
		if (GetFileAttributesA(m_szPath) != FILE_ATTRIBUTE_DIRECTORY)//目录不存在创建
		{
			CreateDirectoryA(m_szPath, NULL);
		}
	}
	~CFileLog(void)
	{
		if(pfp!=NULL)
		{
			fclose(pfp);
		}
		// 释放临界区
		DeleteCriticalSection(&m_cs);
	}
private:
	static HMODULE GetSelfModuleHandle()
	{
		MEMORY_BASIC_INFORMATION mbi;
		return ((::VirtualQuery(GetSelfModuleHandle,&mbi,sizeof(mbi))!= 0)?(HMODULE) mbi.AllocationBase:NULL);
	}
private:
	// Log路径
	char m_szPath[MAX_PATH];
	// Log路径长度
	int m_nPath;
	FILE* pfp;
	// 当前小时
	int m_CurDay;
	int m_nLogLevel;// 0 致命错误,1 一般错误,2..调试信息
	// 临界区
	CRITICAL_SECTION m_cs;
public:
	// 设置Log级别
	int SetLogLevel(int nLevel)
	{
		m_nLogLevel=nLevel;
		return m_nLogLevel;
	}
	bool GetLogLevel(int nLevel)
	{
		return nLevel>m_nLogLevel;
	}
	// 写Log
	bool WriteLog(int nLevel,char* Fmt,...)
	{
		if (nLevel>m_nLogLevel)
		{
			return false;
		}
		try
		{
			char sztBuf[log_buf_size];
			time_t curTime;
			struct tm mt;
			curTime = time(NULL);
			localtime_s(&mt,&curTime);
			strftime(sztBuf,log_buf_size,"%Y-%m-%d %H:%M:%S Level:",&mt);
			int nLen=strlen(sztBuf);
			sprintf_s(sztBuf+nLen,log_buf_size-nLen,"%d,",nLevel);
			nLen=strlen(sztBuf);
			va_list argptr;          //分析字符串的格式
			va_start(argptr, Fmt);
			_vsnprintf_s(sztBuf+nLen, log_buf_size-nLen,_TRUNCATE, Fmt, argptr);
			va_end(argptr);
			if(mt.tm_hour!=m_CurDay) // 每小时一个Log文件
			{
				if (GetFileAttributesA(m_szPath) != FILE_ATTRIBUTE_DIRECTORY)//目录不存在创建
				{
					if(!CreateDirectoryA(m_szPath, NULL))
					{
						return false;
					}
				}
				m_CurDay=mt.tm_hour;	
				char szLogFile[MAX_PATH]={0};
				strcpy_s(szLogFile,MAX_PATH,m_szPath);
				strftime(szLogFile+m_nPath,MAX_PATH-m_nPath,"%Y%m%d%H.log",&mt);
				if (NULL!=pfp)
				{
					fclose(pfp);
					pfp=NULL;
				}
				pfp=_fsopen(szLogFile, "a",_SH_DENYNO); //以添加的方式输出到文件
				if (NULL==pfp)
				{
					return false;
				}
			}
			fprintf(pfp,"%s\n",sztBuf);
			fflush(pfp);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}
	/*************************************
	支持写入覆盖,分月份覆盖
	**************************************/
	bool WriteLog(char* szLog,struct tm* mt)
	{
			EnterCriticalSection(&m_cs);
			if(!pfp||mt->tm_mday!=m_CurDay) // 每小时一个Log文件
			{
				m_CurDay=mt->tm_mday;	
				char szLogFile[MAX_PATH]={0};
				strcpy_s(szLogFile,MAX_PATH,m_szPath);
				strftime(szLogFile+m_nPath,MAX_PATH-m_nPath,"%d.log",mt);
				// 判断文件是否存在
				if (NULL!=pfp)
				{
					fclose(pfp);
					pfp=NULL;
				}
				if(_taccess(szLogFile,0)!= -1 )// 文件存在  
				{
					// 获取文件夹创建时间  
					struct stat sb;
					sb.st_size=sizeof(sb);
					stat(szLogFile,&sb);
					if (sb.st_ctime<time(NULL)-86400)
					{
						DeleteFile(szLogFile);
					}
				}
				pfp=_fsopen(szLogFile, "a",_SH_DENYNO); //以添加的方式输出到文件
				if (NULL==pfp)
				{
					LeaveCriticalSection(&m_cs);
					return false;
				}
			}
			if(fprintf(pfp,"%s\n",szLog)<0)
			{
				fclose(pfp);
				pfp=NULL;
				LeaveCriticalSection(&m_cs);
				return false;
			}
			if(EOF==fflush(pfp))
			{
				fclose(pfp);
				pfp=NULL;
			}
			LeaveCriticalSection(&m_cs);
		return true;
	}
};
