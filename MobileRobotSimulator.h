// MobileRobotSimulator.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMobileRobotSimulatorApp:
// �� Ŭ������ ������ ���ؼ��� MobileRobotSimulator.cpp�� �����Ͻʽÿ�.
//

class CMobileRobotSimulatorApp : public CWinApp
{
public:
	CMobileRobotSimulatorApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMobileRobotSimulatorApp theApp;