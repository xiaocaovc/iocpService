#include "StdAfx.h"
#include "OverBase.h"

COverBase::COverBase(void)
: m_nSize(0)
,m_pIdle(NULL)
,m_pLink(NULL)
{
	InitializeCriticalSection(&m_cs);
}

COverBase::~COverBase(void)
{
	OVERLAPPED_PLUS* pOv=m_pLink;
	OVERLAPPED_PLUS* pLink=NULL;
	while (pOv)
	{
		TRACE("-------写入数据:%u------\n",*((int*)pOv->m_pBuffer));
		pLink=pOv->pLink;
		delete pOv;
		pOv=pLink;
	}
	DeleteCriticalSection(&m_cs);
}

// 初始化
bool COverBase::Init(int nSize)
{
	m_nSize=nSize;
	m_pIdle=new OVERLAPPED_PLUS();
	m_pIdle->m_pBuffer=new char[m_nSize];
	m_pIdle->m_BufferSize=m_nSize;
	m_pIdle->pNext=NULL;
	m_pIdle->pLink=NULL;
	m_pLink=m_pIdle;
	return true;
}

// 获取空闲数据
void* COverBase::GetOver()
{
	OVERLAPPED_PLUS* pOv=NULL;
	EnterCriticalSection(&m_cs);
	if (m_pIdle)
	{
		pOv=m_pIdle;
		m_pIdle=pOv->pNext;
	}
	else
	{
		pOv=new OVERLAPPED_PLUS();
		pOv->m_pBuffer=new char[m_nSize];
		pOv->m_BufferSize=m_nSize;
		pOv->pNext=NULL;
		pOv->pLink=m_pLink;
		m_pLink=pOv;
	}
	LeaveCriticalSection(&m_cs);
	return pOv;
}

// 释放Over
void COverBase::ReleaseOver(OVERLAPPED_PLUS* pOv)
{
	EnterCriticalSection(&m_cs);
	pOv->pNext=m_pIdle;
	m_pIdle=pOv;
	LeaveCriticalSection(&m_cs);
}
