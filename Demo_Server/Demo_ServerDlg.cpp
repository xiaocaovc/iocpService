
// Demo_ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo_Server.h"
#include "Demo_ServerDlg.h"
#include "IocpServerExp.h"
#include "IocpStudioExp.h"
#include "LinkPool.h"
#include "IocpTcpBaseExp.h"


CLinkPool<SERVER_COMPLETIONKEY> g_KeyPool;
CLinkPool<SERVER_OVER> g_OverPool;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDemo_ServerDlg 对话框




CDemo_ServerDlg::CDemo_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemo_ServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemo_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDemo_ServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_CREATE, &CDemo_ServerDlg::OnBnClickedBtCreate)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BT_TEST, &CDemo_ServerDlg::OnBnClickedBtTest)
END_MESSAGE_MAP()


// CDemo_ServerDlg 消息处理程序

BOOL CDemo_ServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化代码
	m_hIocp=Iocp_Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDemo_ServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDemo_ServerDlg::OnPaint()
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
HCURSOR CDemo_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDemo_ServerDlg::OnBnClickedBtCreate()
{
	if(IocpSer_CreateServer(m_hIocp,6688,DoInMonitor,this))
	{
		GetDlgItem(IDC_BT_CREATE)->EnableWindow(FALSE);
	}
}

void CDemo_ServerDlg::OnClose()
{
	Iocp_Stop();
	CDialog::OnClose();
}
int nSock=123;
void CDemo_ServerDlg::DoInMonitor(BOOL bRes,unsigned long dwBytesTransfered,SOCKET nSocket,
				 char* lpOverlapped,CDemo_ServerDlg* lpVoid)
{
	SERVER_COMPLETIONKEY* pKey=g_KeyPool.GetData();
	pKey->m_Socket=nSocket;
	IocpTcp_BindIocp(pKey->m_Socket,lpVoid->m_hIocp,pKey);
	pKey->m_DoOutFun=(DoFun)DoOutFun;
	SERVER_OVER* pOver=g_OverPool.GetData();
	if (!pOver->m_pBuffer)
	{
		pOver->m_pBuffer=new char[81960];
	}
	// 初始化
	pOver->m_nDataLen=0;
	pOver->m_nLeftBytes=4;
	pOver->m_OpType=RECV_POSTED;
	if(!IocpTcp_PostRecv(nSocket,pOver,pOver->m_pBuffer,4))
	{
		g_OverPool.BackData(pOver);
		IocpTcp_StopSock(nSock);
		g_KeyPool.BackData(pKey);
	}

}
void CDemo_ServerDlg::DoOutFun(BOOL bRes,unsigned long dwBytesTransfered,SERVER_COMPLETIONKEY* lpCompletionKey,
			  SERVER_OVER* lpOverlapped,CDemo_ServerDlg* lpVoid)
{
	if (!lpOverlapped)
	{
		return;
	}
	if (!bRes||dwBytesTransfered==0)
	{
		switch (lpOverlapped->m_OpType)
		{
		case RECV_POSTED:
			{
				TRACE("-----Socket:%u断开.资源回收.-----\n",lpCompletionKey->m_Socket);
				IocpTcp_StopSock(lpCompletionKey->m_Socket);
				g_KeyPool.BackData(lpCompletionKey);
			}
		case SEND_POSTED:
			{
				//lpCompletionKey->ReleaseContext(8);
			}
			break;
		default:
			break;
		}
		g_OverPool.BackData(lpOverlapped);
		return;
	}
	switch (lpOverlapped->m_OpType)
	{
	case SEND_POSTED:
		{
			//lpCompletionKey->ReleaseContext(8);
		}
		break;
	case RECV_POSTED:
		{
			if (0==lpOverlapped->m_nDataLen) // 接收长度数据
			{
				lpOverlapped->m_nLeftBytes-=dwBytesTransfered;
				if (lpOverlapped->m_nLeftBytes>0)
				{
					if(!IocpTcp_PostRecv(lpCompletionKey->m_Socket,
						lpOverlapped,lpOverlapped->m_pBuffer+(4-lpOverlapped->m_nLeftBytes),lpOverlapped->m_nLeftBytes))
					{
						IocpTcp_StopSock(lpCompletionKey->m_Socket);
						g_KeyPool.BackData(lpCompletionKey);
						g_OverPool.BackData(lpOverlapped);
					}
				}
				else
				{
					lpOverlapped->m_nLeftBytes=lpOverlapped->m_nDataLen=*(int*)lpOverlapped->m_pBuffer;
					//TRACE("-----接收数据长度:%u.-----\n",lpOverlapped->m_nDataLen);
					// 接收数据部分
					if(!IocpTcp_PostRecv(lpCompletionKey->m_Socket,lpOverlapped,lpOverlapped->m_pBuffer,lpOverlapped->m_nDataLen))
					{
						IocpTcp_StopSock(lpCompletionKey->m_Socket);
						g_KeyPool.BackData(lpCompletionKey);
						g_OverPool.BackData(lpOverlapped);
					}
				}
			}
			else // 接收数据部分
			{
				lpOverlapped->m_nLeftBytes-=dwBytesTransfered;
				if (lpOverlapped->m_nLeftBytes>0)
				{
					if(!IocpTcp_PostRecv(lpCompletionKey->m_Socket,lpOverlapped,
						lpOverlapped->m_pBuffer+(lpOverlapped->m_nDataLen-lpOverlapped->m_nLeftBytes),lpOverlapped->m_nLeftBytes))
					{
						IocpTcp_StopSock(lpCompletionKey->m_Socket);
						g_KeyPool.BackData(lpCompletionKey);
						g_OverPool.BackData(lpOverlapped);
					}
				}
				else // 完整接收到数据
				{
					TRACE("-----接收数据完毕,时间:%u-----\n",clock());
					// 处理数据
					g_OverPool.BackData(lpOverlapped);
					SERVER_OVER* pOver=g_OverPool.GetData();
					if (!pOver->m_pBuffer)
					{
						pOver->m_pBuffer=new char[81960];
					}
					// 初始化
					pOver->m_nDataLen=0;
					pOver->m_nLeftBytes=4;
					pOver->m_OpType=RECV_POSTED;
					if(!IocpTcp_PostRecv(lpCompletionKey->m_Socket,pOver,pOver->m_pBuffer,4))
					{
						IocpTcp_StopSock(lpCompletionKey->m_Socket);
						g_KeyPool.BackData(lpCompletionKey);
						g_OverPool.BackData(pOver);
					}
				}
			}
		}
	default:
		break;
	}
}
void CDemo_ServerDlg::OnBnClickedBtTest()
{
	int nRes=closesocket(nSock);
}
