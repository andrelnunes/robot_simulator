// MobileRobotSimulatorDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "Robot.h"
#include "OpenCV/cv.h"
#include "OpenCV/highgui.h"
#include "OpenCV/cxcore.h"

// CMobileRobotSimulatorDlg 대화 상자
class CMobileRobotSimulatorDlg : public CDialog
{
// 생성입니다.
public:
	CMobileRobotSimulatorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CMobileRobotSimulatorDlg(void);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOBILEROBOTSIMULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedLoadMap();
	afx_msg void OnBnClickedTurnOn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CStatic m_ctrMainView;
	CButton m_ctrTurnOn;
	CButton m_ctrLoadMap;

public:
	CRobot m_Robot;

	CvvImage m_Drawing_Map;
	CDC *m_pMainViewDC;
	HDC m_hMainViewDC;
	IplImage* m_pMap;
	int m_nMapWidth;
	int m_nMapHeight;
	int m_nViewWidth;
	int m_nViewHeight;
	float m_fMapWtoH;

public:
	void SetViewSize(int width, int height);
	void DrawMap();
	CButton m_ctrRunAlgorithm;
	afx_msg void OnBnClickedRunAlgorithm();
};
