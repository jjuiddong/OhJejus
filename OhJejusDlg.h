#pragma once
#include "afxwin.h"
#include "afxcmn.h"


class COhJejusDlg : public CDialogEx
{
public:
	COhJejusDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_OHJEJUS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	void ExecuteProcess( INOUT SProcessData &procInfo );
	void TerminateProcess( INOUT SProcessData &procInfo );
	void ProcessListing( const ProcessDatas &processes );

	ProcessDatas m_Processes;
	BOOL m_IsRunning;
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStop();
	CListCtrl m_List2;
	afx_msg void OnLvnItemchangedListProcess2(NMHDR *pNMHDR, LRESULT *pResult);
};
