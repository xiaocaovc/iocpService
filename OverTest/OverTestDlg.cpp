
// OverTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OverTest.h"
#include "OverTestDlg.h"
#include "OverPoolExp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COverTestDlg 对话框




COverTestDlg::COverTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COverTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COverTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_CREATE, &COverTestDlg::OnBnClickedBtCreate)
	ON_BN_CLICKED(IDC_BT_GET, &COverTestDlg::OnBnClickedBtGet)
	ON_BN_CLICKED(IDC_BT_REL, &COverTestDlg::OnBnClickedBtRel)
END_MESSAGE_MAP()


// COverTestDlg 消息处理程序

BOOL COverTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Ov_Init(8192);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COverTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COverTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COverTestDlg::OnBnClickedBtCreate()
{
	TRACE("-----耗时:%u-----\n",clock());
	for (int i=0;i<6;i++)
	{
		DWORD dw;
		HANDLE handle=CreateThread(NULL,0,LPTHREAD_START_ROUTINE(OperTh),this,0,&dw);
		if (NULL!=handle)
		{
			CloseHandle(handle);
		}
		Sleep(1);
	}
}
void COverTestDlg::OnBnClickedBtGet()
{
}

void COverTestDlg::OnBnClickedBtRel()
{
}

DWORD COverTestDlg::OperTh(COverTestDlg* pVoid)
{
	static int Num=0;
	int abc=Num;
	Num+=1000;
	int bcd=Num;
	OVERLAPPEDEX* pNum=NULL;
	char data[8192];
	TRACE("-----1,线程:%d耗时:%d-----\n",abc,clock());
	for (int i=0;i<100000;i++)
	{
		pNum=(OVERLAPPEDEX*)Ov_GetOverlapped();
		bcd+=i;
		memcpy(data,&bcd,4);
		memcpy(pNum->m_pBuffer,data,8192);
		Ov_ReleaseOverlapped(pNum);
	}
	TRACE("-----2,线程:%d耗时:%d-----\n",abc,clock());
	return 0;
}
