// TesterDlg.h : ��� ����
//

#pragma once

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
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
};
