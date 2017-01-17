#pragma once
#include "RobotMath.h"
#include "OpenCV/cv.h"
#include "OpenCV/highgui.h"
#include "OpenCV/cxcore.h"

#define ROBOT_RADIUS 0.25f
#define ROBOT_WHEEL2WHEEL_DIST 0.4f

#define ROBOT_STATE_UPDATE_PERIOD	10	//ms

#define MAX_ANGULAR_VELOCITY		PI/3.0f		//rad/s
#define MAX_TRANSLATIONAL_VELOCITY	0.3f		//m/s
#define MAX_WHEEL_ACCELERATION		0.3f		//m/s2


class CRobot
{
public:
	CRobot(void);
public:
	~CRobot(void);
public:
	//for virtual robot state update
	void InitRobot(void);
	void UpdatePose(void);
	void UpdateSensor(void);

	//for algorithm update
	void InitAlgorithm(void);
	bool RunAlgorithm(void);

	//for drawing
	void DrawAll(CDC *pDC);
	void DrawRobot(CDC *pDC);
	void DrawSensor(CDC *pDC);
	void DrawTrajectory(CDC *pDC);
	void DrawGoal(CDC *pDC);
	//void DrawPath(CDC *pDC);

public:
	IplImage* m_pGlobalMap;
	bool m_bTurnOn;
	bool m_bAlgorithmOn;

	POSE m_RobotPose;		//Robot pose at global coordinate
	POSE m_RobotTrajectory[1000];

	float v;				//current translational velocity
	float w;				//current angular velocity
	float v_target;			//Target translational velocity
	float w_target;			//Target rotational velocity

	float v_left;			//current left wheel velocity
	float v_right;			//current right wheel velocity
	float v_left_target;	//Target left wheel velocity
	float v_right_target;	//Target right wheel velocity

	ROBOTINFO m_RobotInfo;		//robot information
	
	float m_Travel_Distance;	// variable for evaluation 
	int m_Travel_Time;		// variable for evaluation
	bool m_btermnal_flage;
};
