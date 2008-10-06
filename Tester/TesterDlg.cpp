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
	, m_myIP(_T("192.168.0.1"))
	, m_sourceIP(_T("0"))
	, m_sourceMask(_T("255.255.255.255"))
	, m_sourcePort(_T("0"))
	, m_destIP(_T("0"))
	, m_destMask(_T("255.255.255.255"))
	, m_destPort(_T("0"))
	, m_word(_T(""))
	, m_protocolType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MY_IP, m_myIP);
	DDX_Control(pDX, IDC_BSTART, m_bStart);
	DDX_Control(pDX, IDC_BSTOP, m_bStop);
	DDX_Text(pDX, IDC_ERULE_SOURCE_IP, m_sourceIP);
	DDX_Text(pDX, IDC_ERULE_SOURCE_MASK, m_sourceMask);
	DDX_Text(pDX, IDC_ERULE_SOURCE_PORT, m_sourcePort);
	DDX_Text(pDX, IDC_ERULE_DEST_IP, m_destIP);
	DDX_Text(pDX, IDC_ERULE_DEST_MASK, m_destMask);
	DDX_Text(pDX, IDC_ERULE_DEST_PORT, m_destPort);
	DDX_Control(pDX, IDC_BRULE_ADD, m_bRuleAdd);
	DDX_Control(pDX, IDC_BRULE_DELETE, m_bRuleDelete);
	DDX_Control(pDX, IDC_LIST_RULE, m_listRules);
	DDX_Control(pDX, IDC_LIST_WORD, m_listWords);
	DDX_Text(pDX, IDC_EWORD, m_word);
	DDX_Control(pDX, IDC_BWORD_ADD, m_bWordAdd);
	DDX_Control(pDX, IDC_BWORD_DELETE, m_bWordDelete);
	DDX_CBIndex(pDX, IDC_COMBO1, m_protocolType);
	DDX_Check(pDX, IDC_CHECK_RULE, m_setting.IPFilter);
	DDX_Check(pDX, IDC_CHECK_WORD, m_setting.WordFilter);
	DDX_Check(pDX, IDC_CHECK_SESSION, m_setting.SessionMonitor);
	DDX_Check(pDX, IDC_CHECK_MONITOR, m_setting.PortMonitor);
	DDX_Text(pDX, IDC_EMAXSESSION, m_setting.MaxSession);
	DDX_Text(pDX, IDC_ENOWSESSION, m_setting.NowSession);
}

BEGIN_MESSAGE_MAP(CTesterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BSTART, &CTesterDlg::OnBnClickedBStart)
	ON_BN_CLICKED(IDC_BSTOP, &CTesterDlg::OnBnClickedBStop)
	ON_BN_CLICKED(IDC_BRULE_ADD, &CTesterDlg::OnBnClickedBRuleAdd)
	ON_BN_CLICKED(IDC_BWORD_ADD, &CTesterDlg::OnBnClickedBwordAdd)
	ON_LBN_SELCANCEL(IDC_LIST_WORD, &CTesterDlg::OnLbnSelcancelListWord)
	ON_LBN_SELCHANGE(IDC_LIST_WORD, &CTesterDlg::OnLbnSelchangeListWord)
	ON_BN_CLICKED(IDC_BWORD_DELETE, &CTesterDlg::OnBnClickedBwordDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RULE, &CTesterDlg::OnLvnItemchangedListRule)
	ON_BN_CLICKED(IDC_BRULE_DELETE, &CTesterDlg::OnBnClickedBruleDelete)
	ON_BN_CLICKED(IDC_CHECK_RULE, &CTesterDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_WORD, &CTesterDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_SESSION, &CTesterDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_MONITOR, &CTesterDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_BUPDATE, &CTesterDlg::OnBnClickedBupdate)
END_MESSAGE_MAP()


// CTesterDlg �޽��� ó����

BOOL CTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	m_myIP = GetLocalIP();
	char ascii[256];
	wcstombs( ascii, m_destIP, 256 );
	m_setting.IP = inet_addr(ascii);

	//we load the IPFilter Driver
	filterDriver.LoadDriver(_T("IpFilterDriver"), _T("System32\\Drivers\\IpFltDrv.sys"), NULL, TRUE);
	//we don't deregister the driver at destructor
	filterDriver.SetRemovable(FALSE);
	helper.LoadDriver(_T("MyDriver"), NULL, NULL, TRUE);

	int order = 0;
	m_listRules.InsertColumn(order++,_T("Protocol"), LVCFMT_LEFT, 90);
	m_listRules.InsertColumn(order++,_T("Source IP"), LVCFMT_LEFT, 110);
	m_listRules.InsertColumn(order++,_T("Mask"), LVCFMT_LEFT, 110);
	m_listRules.InsertColumn(order++,_T("Port"), LVCFMT_LEFT, 80);
	m_listRules.InsertColumn(order++,_T("Dest IP"), LVCFMT_LEFT, 110);
	m_listRules.InsertColumn(order++,_T("Mask"), LVCFMT_LEFT, 110);
	m_listRules.InsertColumn(order++,_T("Port"), LVCFMT_LEFT, 80);

	helper.ReadIo(GET_SETTING, &m_setting, sizeof(FirewallSetting));
	m_setting.MaxSession = 100;
	m_setting.NowSession = 0;
	UpdateData(false);

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


void CTesterDlg::OnBnClickedBStart()
{
	m_bStart.EnableWindow(0);
	m_bStop.EnableWindow(1);
	m_bRuleAdd.EnableWindow(1);
	m_bWordAdd.EnableWindow(1);
	helper.WriteIo(START_IP_HOOK, NULL, 0);
}

void CTesterDlg::OnBnClickedBStop()
{
	m_bStart.EnableWindow(1);
	m_bStop.EnableWindow(0);
	m_bRuleAdd.EnableWindow(0);
	m_bRuleDelete.EnableWindow(0);
	m_bWordAdd.EnableWindow(0);
	m_bWordDelete.EnableWindow(0);
	helper.WriteIo(STOP_IP_HOOK, NULL, 0);
}

void CTesterDlg::OnBnClickedBRuleAdd()
{
	UpdateData();

	IPFilter pf;

	if(m_protocolType == 0)
  		pf.protocol = 0;
	else if(m_protocolType == 1)
  		pf.protocol = 6;
	else
		pf.protocol = 17;

	char ascii[256];
	wcstombs( ascii, m_destIP, 256 );
	pf.destinationIp = inet_addr(ascii);
	wcstombs( ascii, m_sourceIP, 256 );
	pf.sourceIp = inet_addr(ascii);
	wcstombs( ascii, m_destMask, 256 );
	pf.destinationMask = inet_addr(ascii);
	wcstombs( ascii, m_sourceMask, 256 );
	pf.sourceMask = inet_addr(ascii);
	wcstombs( ascii, m_destPort, 256 );
	pf.destinationPort = htons(atoi(ascii));
	wcstombs( ascii, m_sourcePort, 256 );
	pf.sourcePort = htons(atoi(ascii));
	if( AddFilter(pf) )	//send the rule
	{
		int index = m_listRules.GetItemCount();
		int subIndex = 1;
		if(m_protocolType == 0)
			m_listRules.InsertItem(index, _T("All"));
		else if(m_protocolType == 1)
			m_listRules.InsertItem(index, _T("TCP"));
		else
			m_listRules.InsertItem(index, _T("UDP"));
		m_listRules.SetItem(index, subIndex++, LVIF_TEXT, m_sourceIP, 0, 0, 0, 0, 0);
		m_listRules.SetItem(index, subIndex++, LVIF_TEXT, m_sourceMask, 0, 0, 0, 0, 0);
		m_listRules.SetItem(index, subIndex++, LVIF_TEXT, m_sourcePort, 0, 0, 0, 0, 0);
		m_listRules.SetItem(index, subIndex++, LVIF_TEXT, m_destIP, 0, 0, 0, 0, 0);
		m_listRules.SetItem(index, subIndex++, LVIF_TEXT, m_destMask, 0, 0, 0, 0, 0);
		m_listRules.SetItem(index, subIndex++, LVIF_TEXT, m_destPort, 0, 0, 0, 0, 0);
	}
}

BOOL CTesterDlg::AddFilter(IPFilter &pf)
{
	DWORD result = helper.WriteIo(ADD_FILTER, &pf, sizeof(pf));

	if (result != DRV_SUCCESS) 
		return FALSE;
	else
		return TRUE;
}

BOOL CTesterDlg::AddWord(WordFilter &wf)
{
	DWORD result = helper.WriteIo(ADD_WORD, &wf, sizeof(WordFilter));

	if (result != DRV_SUCCESS) 
		return FALSE;
	else
		return TRUE;
}

void CTesterDlg::OnBnClickedBwordAdd()
{
	UpdateData();
	WordFilter wf;
	wf.id = m_listWords.GetCount();
	char ascii[10];
	wcstombs( ascii, m_word, 10 );
	strcpy(wf.word, ascii);
	if( AddWord(wf) )	//send the rule
		m_listWords.AddString(m_word);
	m_word = "";
	UpdateData(false);
}

void CTesterDlg::OnLbnSelcancelListWord()
{
	m_bWordDelete.EnableWindow(0);
}

void CTesterDlg::OnLbnSelchangeListWord()
{
	m_bWordDelete.EnableWindow(1);
}

void CTesterDlg::OnBnClickedBwordDelete()
{
	int sel = m_listWords.GetCurSel();
	CString selected;
	m_listWords.GetText(sel, selected);
	m_listWords.DeleteString(sel);
	m_bWordDelete.EnableWindow(0);
}

void CTesterDlg::OnLvnItemchangedListRule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_bRuleDelete.EnableWindow(1);
	*pResult = 0;
}

void CTesterDlg::OnBnClickedBruleDelete()
{
	POSITION pos = m_listRules.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_listRules.GetNextSelectedItem(pos);
		m_listRules.DeleteItem(nItem);
		m_bRuleDelete.EnableWindow(0);
	}

}

void CTesterDlg::OnBnClickedCheck()
{
	UpdateData();
	DWORD result = helper.WriteIo(SET_SETTING, &m_setting, sizeof(FirewallSetting));
	UpdateData(false);
}
void CTesterDlg::OnBnClickedBupdate()
{
	int now = -1;
	DWORD result = helper.ReadIo(GET_SETTING, &now, sizeof(int));
	TRACE("m_setting now %d, %d\n", now, m_setting.MaxSession);
	UpdateData(false);
}
