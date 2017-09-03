/******************使用说明*****************
 1.单线程使用
 2.注意释放
******************使用说明*****************/
#pragma once
#include  <io.h>
#include <stdio.h>
#include <sys/stat.h>

#define  BUF_SIZE 32767
#ifdef  _UNICODE
#define  stat _wstat
#define	 Strcpy_s wcscpy_s
#else
#define  stat _stat
#define	 Strcpy_s strcpy_s
#endif
class CiIni
{
public:
	CiIni(void)
		:m_pData(NULL)
		,m_nLen(0)
		,m_nPos(0)
	{

	}
	~CiIni(void)
	{
		if (m_pData)
		{
			delete[] m_pData;
			m_pData=NULL;
		}
	}
private:
	unsigned long get_file_size(const TCHAR *filename)
	{
		struct stat buf;
		if(stat(filename, &buf)<0)
		{
			return 0;
		}
		return (unsigned long)buf.st_size;
	}
	static HMODULE GetSelfModuleHandle()
	{
		MEMORY_BASIC_INFORMATION mbi;
		return ((::VirtualQuery(GetSelfModuleHandle,&mbi,sizeof(mbi))!= 0)?(HMODULE) mbi.AllocationBase:NULL);
	}
	TCHAR m_szIniPath[MAX_PATH];
	TCHAR* m_pData;
	int m_nLen;
	int m_nPos;
public:

	// 设置Ini文件路径
	BOOL Load(_In_ TCHAR* szIniFile)
	{
		static TCHAR szFilePath[_MAX_PATH]={0};
		if (':'!=szIniFile[1])
		{
			if (_tcsclen (szFilePath)==0)
			{
				// 本模块的路径
				::GetModuleFileName(GetSelfModuleHandle(), szFilePath, MAX_PATH );
				int nLen=_tcsclen (szFilePath);
				for (int i=nLen;i>0;i--)
				{
					if (szFilePath[i]=='\\'||szFilePath[i]=='/')
					{
						szFilePath[i+1]='\0';
						break;
					}
				}
			}
			_stprintf_s(m_szIniPath,MAX_PATH,_T("%s%s"),szFilePath,szIniFile);
		}
		else
		{
			_stprintf_s(m_szIniPath,MAX_PATH,_T("%s"),szIniFile);
		}

		if(_taccess(m_szIniPath,0)!= -1 )// 判断文件是否存在  
		{
			return TRUE;
		}
		return FALSE;
	}
	// --------------遍历操作--------------------------
	// 读取Section
	bool ReadSection(_Out_ TCHAR** pRes)
	{
		// 文件大小
		if (!m_pData)
		{
			int nfileSize=get_file_size(m_szIniPath);
			nfileSize+=1;
			m_pData=new TCHAR[nfileSize];
			m_nLen=GetPrivateProfileString(NULL,NULL,NULL,m_pData,BUF_SIZE,m_szIniPath);
			if (m_nLen==0)
			{
				delete[] m_pData;
				m_pData=NULL;
				return false;
			}
			m_nPos=0;
		}
		if(m_nLen-m_nPos>0)
		{
			*pRes=(m_pData+m_nPos);
			m_nPos+=_tcsclen (*pRes)+1;
			return true;
		}
		else
		{
			delete[] m_pData;
			m_pData=NULL;
		}
		return false;
	}
	// 获取KeyValue
	bool ReadKeyValue(_Out_ TCHAR** pRes,_In_ LPCTSTR lpSection)
	{
		if (!m_pData)
		{
			int nfileSize=get_file_size(m_szIniPath);
			nfileSize+=1;
			m_pData=new TCHAR[nfileSize];
			m_nLen=GetPrivateProfileSection(lpSection,m_pData,BUF_SIZE,m_szIniPath);
			if (m_nLen==0)
			{
				return false;
			}
			m_nPos=0;
		}
		if(m_nLen-m_nPos>0)
		{
			*pRes =m_pData+m_nPos;
			m_nPos+=_tcsclen (*pRes)+1;
			return true;
		}
		else
		{
			delete[] m_pData;
			m_pData=NULL;
		}
		return false;
	}
	// --------------字符处理--------------------------
	// 获得Key
	BOOL GetKey(_In_ TCHAR* szKeyVal,_Out_ TCHAR* szKey)
	{
		int nLen=_tcsclen (szKeyVal);
		for (int i=0;i<nLen;i++)
		{
			if ('='==szKeyVal[i]) // 查找Key
			{
				Strcpy_s(szKey,MAX_PATH,szKeyVal);
				szKey[i]='\0';
				return TRUE;
			}
		}
		return FALSE;
	}
	// 获得Value
	BOOL GetszValue(_In_ TCHAR* szKeyVal,_Out_ TCHAR* szValue)
	{
		int nLen=_tcsclen (szKeyVal);
		for (int i=0;i<nLen;i++)
		{
			if ('='==szKeyVal[i]) // 查找Key
			{
				Strcpy_s(szValue,MAX_PATH,szKeyVal+i+1);
				return TRUE;
			}
		}
		return FALSE;
	}
	// -------------------INI操作----------------------
	// 添加Section
	BOOL AddSection(_In_ LPCTSTR szSection,_In_ LPCTSTR szKeyValue=_T(""))
	{
		if (szSection==_T(""))
		{
			return FALSE;
		}
		TCHAR* pRes=NULL;
		while(ReadSection(&pRes))
		{
			if (_tcsicmp (szSection,pRes)==0)
			{
				return FALSE;
			}
		}
		return WritePrivateProfileSection(szSection,szKeyValue,m_szIniPath);
	}
	// 删除Section
	BOOL DelSection(_In_ LPCTSTR szSection)
	{
		if (szSection==_T(""))
		{
			return FALSE;
		}
		return WritePrivateProfileSection(szSection,NULL,m_szIniPath);
	}
	// 添加KeyValue,value可以为空
	BOOL AddKeyValue(_In_ LPCTSTR szKey, _In_ LPCTSTR szValue,_In_ LPCTSTR szSection)
	{
		if (szKey==_T("")||szSection==_T(""))
		{
			return FALSE;
		}
		return WritePrivateProfileString(szSection,szKey,szValue,m_szIniPath);
	}
	// 删除Key
	BOOL DelKey(_In_ LPCTSTR szKey, _In_ LPCTSTR szSection)
	{
		if (szKey==_T("")||szSection==_T(""))
		{
			return FALSE;
		}
		return WritePrivateProfileString(szSection,szKey,NULL,m_szIniPath);
	}
	// 编辑
	BOOL EditValue(_In_ LPCTSTR szKey, _In_ LPCTSTR szValue, _In_ LPCTSTR szSection)
	{
		if (szKey==_T("")||szSection==_T(""))
		{
			return FALSE;
		}
		return WritePrivateProfileString(szSection,szKey,szValue,m_szIniPath);
	}
	// 获得szValue
	void GetValue(_In_ LPCTSTR szKey,_Out_ TCHAR* szValue,_In_ LPCTSTR szSection,_In_ LPCTSTR szDefValue=_T(""))
	{
		GetPrivateProfileString(szSection,szKey,szDefValue,szValue,MAX_PATH,m_szIniPath);
	}
	// 获得int Value
	int GetValue(_In_ LPCTSTR szKey,_In_ LPCTSTR szSection,_In_ int nDefValue=0)
	{
		return GetPrivateProfileInt(szSection,szKey,nDefValue,m_szIniPath);
	}
};
