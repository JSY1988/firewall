// TesterDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

// CTesterDlg ��ȭ ����
class CTesterDlg : public CDialog
{
// �����Դϴ�.
public:
	CTesterDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TESTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	BOOL AddFilter(IPFilter &pf);
	BOOL AddWord(WordFilter &wf);


// �����Դϴ�.
protected:
	HICON m_hIcon;
	FilterHelper helper;
	FilterHelper filterDriver;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBStart();
	afx_msg void OnBnClickedBStop();
	afx_msg void OnBnClickedBRuleAdd();
	afx_msg void OnBnClickedBwordAdd();
	afx_msg void OnLbnSelcancelListWord();
	afx_msg void OnLbnSelchangeListWord();
	afx_msg void OnBnClickedBwordDelete();
	afx_msg void OnLvnItemchangedListRule(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBruleDelete();
	afx_msg void OnBnClickedCheck();
	
	CString m_myIP;
	CButton m_bStart;
	CButton m_bStop;
	CString m_sourceIP;
	CString m_sourceMask;
	CString m_sourcePort;
	CString m_destIP;
	CString m_destMask;
	CString m_destPort;
	CButton m_bRuleAdd;
	CButton m_bRuleDelete;
	CListCtrl m_listRules;
	CListBox m_listWords;
	CString m_word;
	CButton m_bWordAdd;
	CButton m_bWordDelete;
	int m_protocolType;
	FirewallSetting m_setting;
};
