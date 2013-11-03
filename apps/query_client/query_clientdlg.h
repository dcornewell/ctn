// query_clientDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQuery_clientDlg dialog

class CQuery_clientDlg : public CDialog
{
// Construction
public:
	CQuery_clientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CQuery_clientDlg)
	enum { IDD = IDD_QUERY_CLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuery_clientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CQuery_clientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPatientSearch();
	afx_msg void OnDblclkPatientList();
	afx_msg void OnMoveStudy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
