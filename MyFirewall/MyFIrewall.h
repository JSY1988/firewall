// Tester.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// MyFirewall:
// �� Ŭ������ ������ ���ؼ��� MyFirewall.cpp�� �����Ͻʽÿ�.
//

class CMyFirewallApp : public CWinApp
{
public:
	CMyFirewallApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMyFirewallApp theApp;