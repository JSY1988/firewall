// TesterDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "FilterHelper.h"
#include "..\\myDriver\\Filter.h"
#include "Tester.h"
#include "TesterDlg.h"

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString GetLocalIP()        // local IP ȹ��
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	CString ip; // ���⿡ lcoal ip�� ����˴ϴ�.
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD( 2, 0 );

	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{
		if( gethostname ( name, sizeof(name)) == 0)
		{
			if((hostinfo = gethostbyname(name)) != NULL)
			{
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}      
		WSACleanup( );
	} 
	return ip;
}

// CTesterDlg ��ȭ ����

CTesterDlg::CTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTesterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTesterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTesterDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTesterDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CTesterDlg �޽��� ó����

BOOL CTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	//we load the IPFilter Driver
	filterDriver.LoadDriver(_T("IpFilterDriver"), _T("System32\\Drivers\\IpFltDrv.sys"), NULL, TRUE);
	//we don't deregister the driver at destructor
	filterDriver.SetRemovable(FALSE);
	DWORD ret = helper.LoadDriver(_T("MyDriver"), NULL, NULL, TRUE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

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
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTesterDlg::OnBnClickedButton1()
{
	helper.WriteIo(START_IP_HOOK, NULL, 0);
}

void CTesterDlg::OnBnClickedButton2()
{
	helper.WriteIo(STOP_IP_HOOK, NULL, 0);
}

void CTesterDlg::OnBnClickedButton3()
{
	UpdateData();

	DWORD result;

	IPFilter pf;

	char ascii[256];
	wcstombs( ascii, GetLocalIP().GetBuffer(), 256 );

  	pf.protocol = 0;									
	pf.destinationIp = inet_addr("222.122.84.250");
	pf.sourceIp = inet_addr(ascii);
	pf.destinationMask = inet_addr("255.255.255.255");
	pf.sourceMask = inet_addr("255.255.255.255");
	pf.destinationPort = htons(0);
	pf.sourcePort = htons(0);
	pf.drop = TRUE;

	result = AddFilter(pf);		//send the rule
}

BOOL CTesterDlg::AddFilter(IPFilter &pf)
{
	//we send the rule to the driver
	DWORD result = helper.WriteIo(ADD_FILTER, &pf, sizeof(pf));

	/*if (result != DRV_SUCCESS) 
		return FALSE;
	else*/
		return TRUE;
}