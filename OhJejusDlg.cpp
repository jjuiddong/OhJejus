
// OhJejusDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "OhJejus.h"
#include "OhJejusDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


enum {
	ID_TIMER_CHECK_PROCESS = 100,
};



COhJejusDlg::COhJejusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COhJejusDlg::IDD, pParent)
,	m_IsRunning(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Processes.reserve(4);
}

void COhJejusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_List);
}

BEGIN_MESSAGE_MAP(COhJejusDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &COhJejusDlg::OnBnClickedButtonStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &COhJejusDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// COhJejusDlg �޽��� ó����

BOOL COhJejusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	::SetWindowPos( GetSafeHwnd(),
		HWND_TOPMOST,
		0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE );

	string errMsg;
	int nArgs;
	LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (nArgs >= 2)
	{
		CString arg = szArglist[ 1];
		CStringA charstr(arg);
		string str = charstr;

		if (!ReadConfigFile( str, m_Processes))
		{
			::AfxMessageBox( _T("Error!! ReadConfigFile") );
		}
		else
		{
			LogPrint( "Success Read Config File" );
		}
	}
	else
	{
		::AfxMessageBox( _T("Error!! config ���� ��θ� ���� ���ڰ��� �־��ּ���.") );
		PostQuitMessage(0);
		return FALSE;
	}

	ProcessListing( m_Processes );

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void COhJejusDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void COhJejusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR COhJejusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/**
 @brief 
 */
void COhJejusDlg::ProcessListing( const ProcessDatas &processes )
{
	BOOST_FOREACH (auto &info, processes)
	{
		CStringA str = info.cmdLine.c_str();
		m_List.AddString( CString(str) );
	}
}


/**
 @brief 
 */
void COhJejusDlg::OnBnClickedButtonStart()
{
	if (m_IsRunning)
	{
		LogPrint( "�̹� �������Դϴ�." );
		return;
	}

	m_IsRunning = TRUE;

	// ���μ��� ����.
	BOOST_FOREACH(auto &info, m_Processes)
	{
		ExecuteProcess(info);
	}

	SetTimer( ID_TIMER_CHECK_PROCESS, g_IntervalTime, NULL);
	LogPrint( "interval time = %d", g_IntervalTime );

}

/**
 @brief �ֱ������� ���μ����� ����ִ��� �˻��Ѵ�.
 */
void COhJejusDlg::OnTimer(UINT_PTR nIDEvent)
{
	BOOST_FOREACH(auto &info, m_Processes)
	{
		DWORD exitCode;
		const BOOL result = GetExitCodeProcess(info.pi.hProcess, &exitCode);
		const DWORD errorCode = GetLastError();
		if(STILL_ACTIVE == exitCode)
		{
			// active
			// nothing~
		}
		else
		{
			// terminate
			// resurvive
			LogPrint( "terminate [%s] process ", info.cmdLine.c_str() );
			ExecuteProcess( info );
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


/**
 @brief ���μ��� ����
			���μ��� ������ �����ؼ� �����Ѵ�.
 */
void COhJejusDlg::ExecuteProcess( INOUT SProcessData &procInfo )
{
	STARTUPINFOA si = {0};
	ZeroMemory(&si,sizeof(si));
	ZeroMemory(&procInfo.pi,sizeof(procInfo.pi));
	si.cb = sizeof(STARTUPINFOA);
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow=SW_SHOW;

	stringstream ss;
	ss << procInfo.cmdLine;

	if (!::CreateProcessA(NULL, (char*)ss.str().c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &procInfo.pi))
	{
		return;
	}
}


/**
 @brief 
 */
void COhJejusDlg::TerminateProcess( INOUT SProcessData &procInfo )
{
	::TerminateProcess( procInfo.pi.hProcess, 0 );
}


/**
 @brief ��ü ���α׷� ����.
 */
void COhJejusDlg::OnBnClickedButtonStop()
{
	BOOST_FOREACH(auto &info, m_Processes)
	{
		TerminateProcess(info);
	}

	KillTimer( ID_TIMER_CHECK_PROCESS );
	m_IsRunning = FALSE;
}
