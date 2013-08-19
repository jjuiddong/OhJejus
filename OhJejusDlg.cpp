
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
	DDX_Control(pDX, IDC_LIST_PROCESS2, m_List);
}

BEGIN_MESSAGE_MAP(COhJejusDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &COhJejusDlg::OnBnClickedButtonStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &COhJejusDlg::OnBnClickedButtonStop)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROCESS2, &COhJejusDlg::OnLvnItemchangedListProcess2)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &COhJejusDlg::OnBnClickedButtonReload)
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
	::SetWindowPos( GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE );

	m_List.SetExtendedStyle( m_List.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_List.InsertColumn( 0, L"Process" );
	m_List.SetColumnWidth(0, 250 );
	m_List.InsertColumn( 1, L"State" );
	m_List.SetColumnWidth(1, 60 );

	InitConfigFile();

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
	m_List.DeleteAllItems();

	BOOST_FOREACH (auto &info, processes)
	{
		CStringA str = info.cmdLine.c_str();
		const int item = m_List.InsertItem( m_List.GetItemCount(), CString(str) , 0 );
		m_List.SetCheck( item, info.checkProcess );
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
	for (u_int i=0; i < m_Processes.size(); ++i)
	{
		auto &info = m_Processes[ i];

		DWORD exitCode = 0;
		if (info.pi.hProcess)
			GetExitCodeProcess(info.pi.hProcess, &exitCode);
		//const DWORD errorCode = GetLastError();

		if(STILL_ACTIVE == exitCode)
		{
			info.state = ALIVE;
			m_List.SetItemText( i, 1, L"Alive" );
		}
		else
		{
			// terminate
			// resurvive
			if (info.checkProcess)
			{
				LogPrint( "terminate [%s] process ", info.cmdLine.c_str() );
				ExecuteProcess( info );
				info.state = ALIVE;
				m_List.SetItemText( i, 1, L"Wake" );
			}
			else
			{
				info.state = DEAD;
				m_List.SetItemText( i, 1, L"Dead" );
			}
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
	if (!procInfo.checkProcess)
		return;

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
	for (u_int i=0; i < m_Processes.size(); ++i)
	{
		TerminateProcess(m_Processes[ i]);
		m_List.SetItemText( i, 1, L"Dead" );
	}

	KillTimer( ID_TIMER_CHECK_PROCESS );
	m_IsRunning = FALSE;
}



void COhJejusDlg::OnLvnItemchangedListProcess2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (pNMLV->uOldState == 0 && pNMLV->uNewState == 0)
		return;    // No change

	// Old check box state
	BOOL bPrevState = (BOOL)(((pNMLV->uOldState & LVIS_STATEIMAGEMASK)>>12)-1);  
	if (bPrevState < 0)    // On startup there's no previous state 
		bPrevState = 0; // so assign as false (unchecked)

	// New check box state
	BOOL bChecked = (BOOL)(((pNMLV->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
	if (bChecked < 0) // On non-checkbox notifications assume false
		bChecked = 0; 

	if (bPrevState == bChecked) // No change in check box
		return;

	m_Processes[ pNMLV->iItem].checkProcess = (bChecked? true : false);
}


/**
@brief  스크립트를 재로딩한다.
*/
void COhJejusDlg::OnBnClickedButtonReload()
{
	InitConfigFile();
}


/**
@brief  config 파일을 읽고, 리스트를 초기화 한다.
*/
bool COhJejusDlg::InitConfigFile()
{
	BOOST_FOREACH (auto &proc, m_Processes)
		proc.data = 0;

	ProcessDatas tempData;
	int nArgs;
	LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (nArgs >= 2)
	{
		CString arg = szArglist[ 1];
		CStringA charstr(arg);
		string str = charstr;

		//m_Processes.clear();
		if (!ReadConfigFile( str, tempData))
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
		return false;
	}

	// 정상적으로 읽기를 완료했다면, 현재 실행 중인 프로세스와 비교후에 추가한다.
	// 같은 이름의 프로세스는 추가 되지 않는다.
	// 컨피그 파일에 없는 프로세스는 실행 중이라면, 그대로 두고, 그렇지 않다면 제거한다.
	BOOST_FOREACH (auto &info, tempData)
	{
		info.data = 0;
		info.state = ALIVE; // 살아있은 프로세스를 먼저 탐색한다.
		auto it = std::find(m_Processes.begin(), m_Processes.end(), info);
		if (m_Processes.end() == it)
		{
			info.checkProcess = false;
			m_Processes.push_back( info );
			m_Processes.back().data = 1; // 검색에서 제외 시킬것.
		}
		else
		{
			it->data = 1; // 한번 탐색된 프로세스는 다시 탐색되지 않게 하기 위함.
		}
	}

	// 스크립트에 없는 프로세스는 제거한다. (만약 실행 중이라면 그대로 둔다.)
	for (int i=m_Processes.size()-1; i >= 0; --i)
	{
		if ((m_Processes[ i].data==0) && (DEAD == m_Processes[ i].state))
		{
			m_Processes.erase( m_Processes.begin() + i );
		}
	}

	ProcessListing( m_Processes );
	return true;
}
