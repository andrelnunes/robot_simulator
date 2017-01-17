// MobileRobotSimulatorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MobileRobotSimulator.h"
#include "MobileRobotSimulatorDlg.h"
#include "RobotMath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SCREEN_SIZE 600
#define DEFAULT_MAP_SIZE 1000
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMobileRobotSimulatorDlg 대화 상자




CMobileRobotSimulatorDlg::CMobileRobotSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMobileRobotSimulatorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMap = NULL;
}

CMobileRobotSimulatorDlg::~CMobileRobotSimulatorDlg(void)
{
	if(m_pMap)
		cvReleaseImage(&m_pMap);
}

void CMobileRobotSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_VIEW, m_ctrMainView);
	DDX_Control(pDX, IDC_TURN_ON, m_ctrTurnOn);
	DDX_Control(pDX, IDC_LOAD_MAP, m_ctrLoadMap);
	DDX_Control(pDX, IDC_RUN_ALGORITHM, m_ctrRunAlgorithm);
}

BEGIN_MESSAGE_MAP(CMobileRobotSimulatorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
//	ON_WM_KEYDOWN()
ON_BN_CLICKED(IDC_LOAD_MAP, &CMobileRobotSimulatorDlg::OnBnClickedLoadMap)
ON_BN_CLICKED(IDC_TURN_ON, &CMobileRobotSimulatorDlg::OnBnClickedTurnOn)
ON_WM_TIMER()
ON_WM_KEYDOWN()
ON_BN_CLICKED(IDC_RUN_ALGORITHM, &CMobileRobotSimulatorDlg::OnBnClickedRunAlgorithm)
END_MESSAGE_MAP()


// CMobileRobotSimulatorDlg 메시지 처리기

BOOL CMobileRobotSimulatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(&wndTop, 0,0, SCREEN_SIZE + 30, SCREEN_SIZE + 150, SWP_SHOWWINDOW);
	m_ctrMainView.SetWindowPos(&wndTop, 10, 10, SCREEN_SIZE, SCREEN_SIZE, SWP_SHOWWINDOW);
	m_ctrLoadMap.SetWindowPos(&wndTop, 10, SCREEN_SIZE+50, 120, 40, SWP_SHOWWINDOW);
	m_ctrTurnOn.SetWindowPos(&wndTop, 140, SCREEN_SIZE+50, 120, 40, SWP_SHOWWINDOW);
	m_ctrRunAlgorithm.SetWindowPos(&wndTop, 270, SCREEN_SIZE+50, 120, 40, SWP_SHOWWINDOW);

	m_pMainViewDC = m_ctrMainView.GetDC();
	m_hMainViewDC = m_pMainViewDC->GetSafeHdc();

	//Set Transform Parameters
	SetParameter(DEFAULT_MAP_SIZE, DEFAULT_MAP_SIZE, SCREEN_SIZE, SCREEN_SIZE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMobileRobotSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMobileRobotSimulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMobileRobotSimulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMobileRobotSimulatorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if((pMsg->message == WM_KEYDOWN ||pMsg->message == WM_SYSKEYDOWN )&&
		(pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP || 
		 pMsg->wParam == VK_DOWN || pMsg->wParam == VK_SPACE)){
			SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMobileRobotSimulatorDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nChar){
		case VK_UP:		//When you press 'up' key
			m_Robot.v_target = m_Robot.v + 0.3f;
			break;
		case VK_DOWN:	//When you press 'down' key
			m_Robot.v_target = m_Robot.v - 0.3f;
			break;
		case VK_RIGHT:	//When you press 'right' key
			m_Robot.w_target = -PI/3.0f;
			break;
		case VK_LEFT:	//When you press 'left' key
			m_Robot.w_target = PI/3.0f;
			break;
		case VK_SPACE:	//When you press 'space' key (stop)
			m_Robot.v_target = 0;
			m_Robot.w_target = 0;
			break;
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMobileRobotSimulatorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent){
		case 0:		//10 ms
			if(m_Robot.m_bTurnOn)
				m_Robot.UpdatePose();
			break;
		case 1:		//200 ms
			if(m_pMap)
				DrawMap();
			if(m_Robot.m_bTurnOn){
				m_Robot.UpdateSensor();
				m_Robot.DrawAll(m_pMainViewDC);
			}
			break;
		/*case 2:
			if(m_Robot.m_bAlgorithmOn){
				m_Robot.RunAlgorithm();
			}
			break;*/
		case 2:		//algorithm  (100 ms)
			if(m_Robot.m_bAlgorithmOn){
				bool temination_flag = m_Robot.RunAlgorithm();

				if( !temination_flag & m_Robot.m_bAlgorithmOn ){
					m_Robot.m_bAlgorithmOn = FALSE;
					KillTimer(2);					
				}
			}
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMobileRobotSimulatorDlg::OnBnClickedLoadMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString path, ext;
	CFileDialog* dlg = new CFileDialog(true , NULL , NULL , OFN_HIDEREADONLY , NULL,NULL,0);// (true, "map", "Map", OFN_HIDEREADONLY, NULL);)
	IplImage* tmpMap=NULL;

	if(dlg->DoModal() == IDOK)
	{
		ext = dlg->GetFileExt();

		if(ext =="BMP" || ext=="bmp" || ext=="JPG" || ext =="jpg"){


			path = dlg->GetPathName();
			if(m_pMap)
				cvReleaseImage(&m_pMap);
			m_pMap = cvLoadImage(path, 0);
			if(m_pMap){
				m_nMapWidth = m_pMap->width;
				m_nMapHeight = m_pMap->height;
				m_fMapWtoH = (float)m_nMapWidth/(float)m_nMapHeight;
				SetViewSize(m_nMapWidth, m_nMapHeight);
				SetParameter(m_nMapWidth, m_nMapHeight, m_nViewWidth, m_nViewHeight);
				m_Robot.m_pGlobalMap = m_pMap;

				if(m_pMap->nChannels==1){
					tmpMap = cvCreateImage(cvGetSize(m_pMap), 8, 3);
					cvCvtColor(m_pMap, tmpMap, CV_GRAY2BGR);
					m_Drawing_Map.CopyOf(tmpMap);
				}
				else{
					m_Drawing_Map.CopyOf(m_pMap);
				}
				if(tmpMap)
					cvReleaseImage(&tmpMap);
			}

			//set timer for drawing
			SetTimer(1, 200, NULL);			//draw  (200 ms)
		}
	}
	else
		return;

	delete dlg;
}

void CMobileRobotSimulatorDlg::SetViewSize(int width, int height)
{
	if(width > SCREEN_SIZE || height > SCREEN_SIZE){
		if(m_fMapWtoH >= 1.0){
			m_nViewWidth = SCREEN_SIZE;
			m_nViewHeight = (int)(m_nViewWidth/m_fMapWtoH);
		}
		else{
			m_nViewHeight= SCREEN_SIZE;
			m_nViewWidth = (int)(m_nViewHeight*m_fMapWtoH);
		}
	}

	else{
		if(m_fMapWtoH >= 1.0){
			m_nViewWidth = width;
			m_nViewHeight = (int)(m_nViewWidth/m_fMapWtoH);
		}
		else{
			m_nViewHeight= height;
			m_nViewWidth = (int)(m_nViewHeight*m_fMapWtoH);
		}
	}

	m_ctrMainView.SetWindowPos(&wndTop, 10, 10, m_nViewWidth, m_nViewHeight, SWP_SHOWWINDOW);
}

void CMobileRobotSimulatorDlg::DrawMap()
{
	CRect rect;
	m_ctrMainView.GetClientRect(&rect);
	m_Drawing_Map.DrawToHDC(m_hMainViewDC,&rect);
}

void CMobileRobotSimulatorDlg::OnBnClickedTurnOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_Robot.m_bTurnOn){
		m_Robot.m_bTurnOn = TRUE;
		//Set timer
		SetTimer(0, ROBOT_STATE_UPDATE_PERIOD, NULL);	//update robot state (10 ms)
	}
	else{
		m_Robot.InitRobot();
		//Kill timer
		KillTimer(0);
	}
}

void CMobileRobotSimulatorDlg::OnBnClickedRunAlgorithm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_Robot.m_bTurnOn){
		AfxMessageBox("Please turn on the robot!");
	}
	if(!m_pMap){
		AfxMessageBox("Please load map!");
	}
	if(!m_Robot.m_bAlgorithmOn){
		if(m_Robot.m_bTurnOn && m_pMap){
			m_Robot.m_bAlgorithmOn = TRUE;

			//initialize algorithm
			m_Robot.InitAlgorithm();

			//set timer
			SetTimer(2, 100, NULL);		//algorithm  (100 ms)
		}
	}
	else{
		AfxMessageBox("Algorithm is already running!");
	}
}
