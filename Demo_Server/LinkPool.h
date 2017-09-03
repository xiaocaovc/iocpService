/***********************说明***********************
可复用的链表结构.获取,归还,分管理链表,和空闲链表,链表
本身无序.
支持多线程获取,归还.
特别的,归还时,使用者要防止同一元素归还两次或多次.
***********************说明***********************/
#pragma once

// 结构范例
#define  LINK_DATA char

typedef struct LINK_STRU
{
	LINK_DATA  Data;
	LINK_STRU* pLink;
	LINK_STRU* pNext;
};
template<class T>
class CLinkPool
{
public:
	CLinkPool(void)
	:m_pLink(NULL)
	,m_pIdle(NULL)
	{
		InitializeCriticalSection(&m_cs);
	}
	~CLinkPool(void)
	{
		int i=0;
		T* pData=m_pLink;
		T* pLink=NULL;
		while (pData)
		{
			pLink=(T*)pData->pLink;
			delete pData;
			pData=pLink;
			i++;
		}
		TRACE("-----Link:%d-----\n",i);
		DeleteCriticalSection(&m_cs);
	}
private:
	CRITICAL_SECTION m_cs;
	// 数据大小
	int m_nSize;
	// 空闲数据头指针
	T* m_pIdle;
	// 数据链头
	T* m_pLink;
public:
public:
	// 获得一个Key
	T* GetData(void)
	{
		T* pData=NULL;
		EnterCriticalSection(&m_cs);
		if (m_pIdle) // 空闲链表不为空
		{
			pData=m_pIdle;
			m_pIdle=(T*)pData->pNext;
		}
		else
		{
			pData=new T();
			pData->pNext=NULL;
			pData->pLink=m_pLink; // 加入管理链表
			m_pLink=pData;
		}
		LeaveCriticalSection(&m_cs);
		return pData;
	}
	// 归还Data,放入空闲链表中
	void BackData(T* pData)
	{
		EnterCriticalSection(&m_cs);
		pData->pNext=m_pIdle;
		m_pIdle=pData;
		LeaveCriticalSection(&m_cs);
	}
};
