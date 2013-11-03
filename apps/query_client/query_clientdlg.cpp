// query_clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "query_client.h"
#include "query_clientDlg.h"

#include "dicom.h"
#include "lst.h"
#include "condition.h"

extern "C" {
#include "query_interface.h"
#include "format.h"
};

static LST_HEAD *patientList = NULL;
static LST_HEAD *studyList = NULL;
static PATIENT_QUERY_MODULE selectedPatient;
//static STUDY_QUERY_MODULE selectedStudy;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuery_clientDlg dialog

CQuery_clientDlg::CQuery_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuery_clientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuery_clientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQuery_clientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuery_clientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQuery_clientDlg, CDialog)
	//{{AFX_MSG_MAP(CQuery_clientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Search, OnPatientSearch)
	ON_LBN_DBLCLK(IDC_PatientList, OnDblclkPatientList)
	ON_BN_CLICKED(IDC_Move, OnMoveStudy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuery_clientDlg message handlers

BOOL CQuery_clientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQuery_clientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQuery_clientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQuery_clientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void fillConnectionModule(HWND w, CONNECTION_MODULE *m)
{
  ::GetDlgItemText(w, IDC_ImageServerHost,
				m->NodeName, sizeof(m->NodeName)-1);
  ::GetDlgItemText(w, IDC_ImageServerPort,
				m->PortNumber, sizeof(m->PortNumber)-1);
  ::GetDlgItemText(w, IDC_CallingAETitle,
				m->CallingAPTitle, sizeof(m->CallingAPTitle)-1);
  ::GetDlgItemText(w, IDC_CalledAETitle,
				m->CalledAPTitle, sizeof(m->CalledAPTitle)-1);
}

void fillSearchCriteria(HWND w, PATIENT_QUERY_MODULE *p)
{
	::GetDlgItemText(w, IDC_PatientID, p->PatientID, sizeof(p->PatientID)-1);
	::GetDlgItemText(w, IDC_PatientName,
		p->LastName, sizeof(p->LastName)-1);
	::GetDlgItemText(w, IDC_BirthDate,
		p->BirthDate, sizeof(p->BirthDate)-1);
}

static void listClear(LST_HEAD** lst)
{
	  LST_NODE *n;

	  while ((n = LST_Dequeue(lst)) != NULL)
		  free(n);
}

static void loadList(HWND w, int listID, LST_HEAD* lst,
					 void (*format)(void *n, int index, char* buf), char* buf)
{
  LST_NODE* n;
  int index = 1;

  ::SendDlgItemMessage(w, listID, LB_RESETCONTENT, 0, 0);

  n = LST_Head(&lst);
  (void)LST_Position(&lst, n);
  while (n != NULL) {
	format(n, index, buf);
	::SendDlgItemMessage(w, listID,
			 LB_ADDSTRING, 0,
			 (LPARAM)buf);
	n = LST_Next(&lst);
  }

}

void CQuery_clientDlg::OnPatientSearch() 
{
  HWND w;
  CONNECTION_MODULE connect;
  PATIENT_QUERY_MODULE query;
  char buf[128];

  w = ::GetActiveWindow();

  fillConnectionModule(w, &connect);
  fillSearchCriteria(w, &query);

  if (patientList == NULL)
	  patientList = LST_Create();
  if (studyList == NULL)
	  studyList = LST_Create();

  listClear(&patientList);

  patientQuery(&connect, &query, &patientList);
  loadList(w, IDC_PatientList, patientList, formatPatient, buf);



}

void CQuery_clientDlg::OnDblclkPatientList() 
{
	// TODO: Add your control notification handler code here
	int i;
	HWND w;
	CONNECTION_MODULE connect;
	CONDITION cond;
	char buf[512];

	w = ::GetActiveWindow();

	i = ::SendDlgItemMessage(w, IDC_PatientList, LB_GETCURSEL, 0, 0);

	PATIENT_QUERY_MODULE* p;

	p = (PATIENT_QUERY_MODULE*)LST_Head(&patientList);
	(void)LST_Position(&patientList, p);
	while (i-- > 0)
		p = (PATIENT_QUERY_MODULE*)LST_Next(&patientList);

    fillConnectionModule(w, &connect);

	listClear(&studyList);
	cond = studyQuery(&connect, p, &studyList);

	loadList(w, IDC_StudyList, studyList, formatStudy, buf);

	selectedPatient = *p;
}

void CQuery_clientDlg::OnMoveStudy() 
{
	// TODO: Add your control notification handler code here
    int i;
	HWND w;
	CONNECTION_MODULE connect;
	CONDITION cond;
	char moveDestination[128];

	w = ::GetActiveWindow();

	i = ::SendDlgItemMessage(w, IDC_StudyList, LB_GETCURSEL, 0, 0);

	if (i == LB_ERR)
		return;

	STUDY_QUERY_MODULE* s;

	s = (STUDY_QUERY_MODULE*)LST_Head(&studyList);
	(void)LST_Position(&studyList, s);
	while (i-- > 0)
		s = (STUDY_QUERY_MODULE*)LST_Next(&studyList);

	::GetDlgItemText(w, IDC_MoveDestination,
				moveDestination, sizeof(moveDestination)-1);

	w = ::GetActiveWindow();
    fillConnectionModule(w, &connect);

	cond = moveRequest(&connect, moveDestination,
		&selectedPatient, s);

	(void)COND_PopCondition(TRUE);
}
