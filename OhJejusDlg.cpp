
// OhJejusDlg.cpp : 구현 파일
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


// COhJejusDlg 메시지 처리기

BOOL COhJejusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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
		::AfxMessageBox( _T("Error!! config 파일 경로를 실행 인자값에 넣어주세요.") );
		PostQuitMessage(0);
		return FALSE;
	}

	ProcessListing( m_Processes );

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COhJejusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
		LogPrint( "이미 실행중입니다." );
		return;
	}

	m_IsRunning = TRUE;

	// 프로세스 실행.
	BOOST_FOREACH(auto &info, m_Processes)
	{
		ExecuteProcess(info);
	}

	SetTimer( ID_TIMER_CHECK_PROCESS, g_IntervalTime, NULL);
	LogPrint( "interval time = %d", g_IntervalTime );

}

/**
 @brief 주기적으로 프로세스가 살아있는지 검사한다.
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
 @brief 프로세스 실행
			프로세스 정보를 저장해서 리턴한다.
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
 @brief 전체 프로그램 종료.
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
