
// Demo_ServerDlg.h : 头文件
//

#pragma once

#include "BusStruct.h"

// CDemo_ServerDlg 对话框
class CDemo_ServerDlg : public CDialog
{
// 构造
public:
	CDemo_ServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DEMO_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtCreate();
	HANDLE m_hIocp;
	afx_msg void OnClose();
	static void DoInMonitor(BOOL bRes,unsigned long dwBytesTransfered,SOCKET nSocket,
		char* lpOverlapped,CDemo_ServerDlg* lpVoid);

	static void DoOutFun(BOOL bRes,unsigned long dwBytesTransfered,SERVER_COMPLETIONKEY* lpCompletionKey,
		SERVER_OVER* lpOverlapped,CDemo_ServerDlg* lpVoid);

	afx_msg void OnBnClickedBtTest();
};
