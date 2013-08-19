
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
	::SetWindowPos( GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE );

	m_List.SetExtendedStyle( m_List.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_List.InsertColumn( 0, L"Process" );
	m_List.SetColumnWidth(0, 250 );
	m_List.InsertColumn( 1, L"State" );
	m_List.SetColumnWidth(1, 60 );

	InitConfigFile();

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
 @brief ���μ��� ����
			���μ��� ������ �����ؼ� �����Ѵ�.
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
 @brief ��ü ���α׷� ����.
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
@brief  ��ũ��Ʈ�� ��ε��Ѵ�.
*/
void COhJejusDlg::OnBnClickedButtonReload()
{
	InitConfigFile();
}


/**
@brief  config ������ �а�, ����Ʈ�� �ʱ�ȭ �Ѵ�.
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
		::AfxMessageBox( _T("Error!! config ���� ��θ� ���� ���ڰ��� �־��ּ���.") );
		PostQuitMessage(0);
		return false;
	}

	// ���������� �б⸦ �Ϸ��ߴٸ�, ���� ���� ���� ���μ����� ���Ŀ� �߰��Ѵ�.
	// ���� �̸��� ���μ����� �߰� ���� �ʴ´�.
	// ���Ǳ� ���Ͽ� ���� ���μ����� ���� ���̶��, �״�� �ΰ�, �׷��� �ʴٸ� �����Ѵ�.
	BOOST_FOREACH (auto &info, tempData)
	{
		info.data = 0;
		info.state = ALIVE; // ������� ���μ����� ���� Ž���Ѵ�.
		auto it = std::find(m_Processes.begin(), m_Processes.end(), info);
		if (m_Processes.end() == it)
		{
			info.checkProcess = false;
			m_Processes.push_back( info );
			m_Processes.back().data = 1; // �˻����� ���� ��ų��.
		}
		else
		{
			it->data = 1; // �ѹ� Ž���� ���μ����� �ٽ� Ž������ �ʰ� �ϱ� ����.
		}
	}

	// ��ũ��Ʈ�� ���� ���μ����� �����Ѵ�. (���� ���� ���̶�� �״�� �д�.)
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
