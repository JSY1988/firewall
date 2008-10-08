// TesterDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CMyFirewallDlg ��ȭ ����
class CMyFirewallDlg : public CDialog
{
// �����Դϴ�.
public:
	CMyFirewallDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TESTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	BOOL AddFilter(IPFilter &pf);
	BOOL AddWord(WordFilter &wf);
	void UpdatePorts();
	DWORD SendSetting();
	void GetLocalIP();


// �����Դϴ�.
protected:
	HICON m_hIcon;
	DriverHelper helper;
	DriverHelper filterDriver;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedBStart();
	afx_msg void OnBnClickedBStop();
	afx_msg void OnBnClickedBRuleAdd();
	afx_msg void OnBnClickedBwordAdd();
	afx_msg void OnBnClickedBwordDelete();
	afx_msg void OnBnClickedBruleDelete();
	afx_msg void OnLvnItemchangedListRule(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListWord(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListPort(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	CString m_myIP;
	CString m_sourceIP;
	CString m_sourceMask;
	CString m_sourcePort;
	CString m_destIP;
	CString m_destMask;
	CString m_destPort;
	CListCtrl m_listRules;
	CListCtrl m_listWords;
	CString m_word;
	int m_protocolType;
	CListCtrl m_listPorts;
	PortsManager m_portsManager;
	const int update_interval;
	CChartCtrl m_chartCtrl;
	BOOL m_IPFilter;
	BOOL m_wordFilter;
	BOOL m_sessionFilter;
	BOOL m_portMonitor;
	bool m_exceed;
	ULONG m_IP;
	int m_maxSession;
	int m_nowSession;
	int m_total;
	int m_idRule;
	int m_idWord;
	bool m_started;
};
