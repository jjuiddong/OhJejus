
// OhJejus.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// COhJejusApp:
// �� Ŭ������ ������ ���ؼ��� OhJejus.cpp�� �����Ͻʽÿ�.
//

class COhJejusApp : public CWinApp
{
public:
	COhJejusApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern COhJejusApp theApp;