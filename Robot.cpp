#include "StdAfx.h"
#include "Robot.h"
#include "Algorithm.h"

CRobot::CRobot(void)
{
	InitRobot();
}

CRobot::~CRobot(void)
{
}

void CRobot::InitRobot(void)
{
	m_pGlobalMap = NULL;

	m_bTurnOn = FALSE;
	m_bAlgorithmOn = FALSE;

	//set initial pose
	m_RobotPose.x = -4.0f;
	m_RobotPose.y = -4.0f;
	m_RobotPose.theta = 0.0f;

	//set v, w velocity
	v = 0.0f;
	w = 0.0f;
	v_target = 0.0f;			//Target translational velocity
	w_target = 0.0f;			//Target rotational velocity

	//set wheel velocity
	v_left = 0.0f;
	v_right = 0.0f;
	v_left_target = 0.0f;	//Target left wheel velocity
	v_right_target = 0.0f;	//Target right wheel velocity

	//set robot info
	m_RobotInfo.pose = m_RobotPose;
	m_RobotInfo.v = v;
	m_RobotInfo.w = w;
	//set sensor value
	
	// initialize trajectory
	for(int i=0; i<1000; i++)
	{
		m_RobotTrajectory[i].x=0;
		m_RobotTrajectory[i].y=0;
	}
	//set evaluation value
	m_Travel_Distance = 0;
	m_Travel_Time = 0;
	m_btermnal_flage = true;
}

void CRobot::UpdatePose(void)
{
	//calculate current velocity 

	if (v_target > 0.3f)		v_target = 0.3f;
	if (v_target < -0.3f)		v_target = -0.3f;
		
	if (w_target > (PI/3.0))		w_target = (PI/3.0);
	if (w_target < -(PI/3.0))		w_target = -(PI/3.0);

	v_left_target = v_target/100 - w_target*(ROBOT_WHEEL2WHEEL_DIST/2)/100;
	v_right_target = v_target/100 + w_target*(ROBOT_WHEEL2WHEEL_DIST/2)/100;

	//2. update (v_left, v_right) using constant acceleration (MAX_WHEEL_ACCELERATION)

	if((v_left < v_left_target))
		v_left = v_left + MAX_WHEEL_ACCELERATION*ROBOT_STATE_UPDATE_PERIOD/1000;
	else if(v_left > v_left_target)
		v_left = v_left - MAX_WHEEL_ACCELERATION*ROBOT_STATE_UPDATE_PERIOD/1000;

	if((v_right < v_right_target))
			v_right = v_right + MAX_WHEEL_ACCELERATION*ROBOT_STATE_UPDATE_PERIOD/1000;
	else if(v_right > v_right_target)
			v_right = v_right - MAX_WHEEL_ACCELERATION*ROBOT_STATE_UPDATE_PERIOD/1000;
	
	//3. update current (v,w)

	v = (v_right + v_left)/2;
	w = (v_right - v_left)/(ROBOT_WHEEL2WHEEL_DIST);

	//update robot pose using (v,w) and (ROBOT_STATE_UPDATE_PERIOD) assume constant value during one-time-step

	m_RobotPose.x = m_RobotPose.x + v*cos(m_RobotPose.theta + w);
	m_RobotPose.y = m_RobotPose.y + v*sin(m_RobotPose.theta + w);
	m_RobotPose.theta = m_RobotPose.theta + w;
	
	if(w_target != 0) w_target = w_target - w;

	// For evaluation
	m_Travel_Distance += sqrt((m_RobotPose.x - m_RobotInfo.pose.x)*(m_RobotPose.x - m_RobotInfo.pose.x) + 
		(m_RobotPose.y - m_RobotInfo.pose.y)*(m_RobotPose.y - m_RobotInfo.pose.y));
	if( m_Travel_Distance != 0 ) {
		m_Travel_Time++;
	}
	//finally set robot info
	m_RobotInfo.pose = m_RobotPose;
	m_RobotInfo.v = v;
	m_RobotInfo.w = w;
	
	/*		for(int i=0;i<=3;i++)
	{
		if ( ( m_RobotInfo.sensorValue[i] > 0) && ( m_RobotInfo.sensorValue[i] <0.3) && (m_RobotInfo.sensorValue[i]!=-1)){
		w_target = 0;
		v_target = 0;
		}
	}

		for(int i=3;i<=6;i++)
	{
		if (( m_RobotInfo.sensorValue[i] > 0) && ( m_RobotInfo.sensorValue[i] <0.3) && (m_RobotInfo.sensorValue[i]!=-1)){
		w_target = 0;
		v_target = 0;
		}
	}*/
}

void CRobot::UpdateSensor(void)
{
	//use following function to get pixel value: x, y should be represented in map coordinate frame
int i, j, x_m, y_m;
float x_l, y_l;
float x_g, y_g;


x_l = 0.0f; y_l = 0.0f; //center point
x_m = 0; y_m = 0;

float x_r_g, y_r_g;
x_r_g = m_RobotPose.x;
y_r_g = m_RobotPose.y;
int x_r_m, y_r_m;

GlobalToMap(x_r_g, y_r_g, x_r_m, y_r_m);

	for(i=0;i<=6;i++)
	{
		m_RobotInfo.sensorValue[i]=-1;

	}

	for(i=0;i<7;i++)
	{
		for(j=0;j<=100;j++)
		{
			x_l = cos(i*PI/6-PI/2)*(ROBOT_RADIUS + j*0.01);
			y_l = sin(i*PI/6-PI/2)*(ROBOT_RADIUS + j*0.01);

			LocalToGlobal(m_RobotPose, x_l, y_l, x_g, y_g);
			GlobalToMap(x_g, y_g, x_m, y_m);

			//int value = cvGetReal2D(m_pGlobalMap, y_m, x_m);
		
			if(cvGetReal2D(m_pGlobalMap, y_m, x_m) == 0)
			{
				m_RobotInfo.sensorValue[i] = (float) (j * 0.01);
				break;
			}
		
			if(j == 100)
			{
				m_RobotInfo.sensorValue[i] = -1;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// initialization of the algorithm
void CRobot::InitAlgorithm(void)
{
	InitializeStartGoalNavigation(m_pGlobalMap, m_RobotInfo);
}

// called every 100 ms
bool CRobot::RunAlgorithm(void)
{
	//call the algorithm function (calculate v_target, w_target)
	//RunStartGoalNavigation(m_pGlobalMap, m_RobotInfo, &v_target, &w_target);
	if( m_btermnal_flage ){
		m_btermnal_flage = RunStartGoalNavigation(m_pGlobalMap, m_RobotInfo, &v_target, &w_target);
		// For evaluation
		if( m_btermnal_flage == false)
		{
			CString str;
			str.Format( "Travel distance : %f\nTravel time : %d", m_Travel_Distance, m_Travel_Time);
			AfxMessageBox( str );

			return false;
		}	
	}

	//finally check maximum velocity
	if(v_target >  MAX_TRANSLATIONAL_VELOCITY) v_target =  MAX_TRANSLATIONAL_VELOCITY;
	if(v_target < -MAX_TRANSLATIONAL_VELOCITY) v_target = -MAX_TRANSLATIONAL_VELOCITY;
	if(w_target >  MAX_ANGULAR_VELOCITY) w_target =  MAX_ANGULAR_VELOCITY;
	if(w_target < -MAX_ANGULAR_VELOCITY) w_target = -MAX_ANGULAR_VELOCITY;
}


//darwing functions///////////////////////////////////////////////////////////////////////////////
void CRobot::DrawAll(CDC *pDC)
{
	DrawTrajectory(pDC);
	DrawSensor(pDC);
	DrawRobot(pDC);
	DrawGoal(pDC);
	//DrawPath(pDC);
}

void CRobot::DrawRobot(CDC *pDC)
{
	CBrush *oldBrush;
	CPen *oldPen;
	float x_g, y_g; //global coordinate
	float x_l, y_l; //local coordinate
	int x_s, y_s; //screen coordinate
	CPoint Pt1, Pt2;
	int PenWidth;

	GlobalToScreen(0.05f, PenWidth);//0.05m width

	CBrush blueBrush(0xff0000);
	CPen redPen(PS_SOLID, 2, 0x0000ff);

	oldPen = pDC->GetCurrentPen();
	oldBrush = pDC->GetCurrentBrush();

	/////// Draw RealPose ///////////////
	//1. draw body
	x_g = m_RobotPose.x-ROBOT_RADIUS; 
	y_g = m_RobotPose.y+ROBOT_RADIUS; //left top point
	GlobalToScreen(x_g, y_g, x_s, y_s);
	Pt1.x = x_s; Pt1.y = y_s;
	x_g = m_RobotPose.x+ROBOT_RADIUS; 
	y_g = m_RobotPose.y-ROBOT_RADIUS; //right bottom point
	GlobalToScreen(x_g, y_g, x_s, y_s);
	Pt2.x = x_s; Pt2.y = y_s;
	pDC->SelectObject(&blueBrush);
	pDC->Ellipse(Pt1.x, Pt1.y, Pt2.x, Pt2.y);
	pDC->SelectObject(oldBrush);

	//2. draw orientation indication bar	
	x_l = 0.0f; y_l = 0.0f; //left top point
	LocalToScreen(m_RobotPose, x_l, y_l, x_s, y_s);
	Pt1.x = x_s; Pt1.y = y_s;
	x_l = ROBOT_RADIUS; y_l = 0.0f; //right bottom point
	LocalToScreen(m_RobotPose, x_l, y_l, x_s, y_s);
	Pt2.x = x_s; Pt2.y = y_s;
	pDC->SelectObject(&redPen);
	pDC->MoveTo(Pt1);
	pDC->LineTo(Pt2);
	pDC->SelectObject(oldPen);
}

void CRobot::DrawGoal(CDC *pDC)
{
	CBrush *oldBrush;
	float x_g, y_g; //global coordinate
	int x_s, y_s; //screen coordinate
	CPoint Pt1, Pt2;

	CBrush blueBrush(0xfff000);
	oldBrush = pDC->GetCurrentBrush();

	/////// Draw RealPose ///////////////
	//1. draw body
	x_g = 4-ROBOT_RADIUS; 
	y_g = 4 +ROBOT_RADIUS; //left top point
	GlobalToScreen(x_g, y_g, x_s, y_s);

	Pt1.x = x_s; Pt1.y = y_s;
	x_g = 4+ROBOT_RADIUS; 
	y_g = 4-ROBOT_RADIUS; //right bottom point
	GlobalToScreen(x_g, y_g, x_s, y_s);
	Pt2.x = x_s; Pt2.y = y_s;
	pDC->SelectObject(&blueBrush);
	pDC->Ellipse(Pt1.x, Pt1.y, Pt2.x, Pt2.y);
	pDC->SelectObject(oldBrush);

}

void CRobot::DrawSensor(CDC *pDC)
{
	int x_m, y_m;
float x_l, y_l;

x_l = 0.0f; y_l = 0.0f; //center point
x_m = 0; y_m = 0;

float x_r_g, y_r_g;
x_r_g = m_RobotPose.x;
y_r_g = m_RobotPose.y;

CBrush *oldBrush;
CPen *oldPen;
int i,x_s, y_s; //screen coordinate
CPoint Pt1, Pt2, Pt3;
int PenWidth;

GlobalToScreen(0.05f, PenWidth);//0.05m width

CBrush blueBrush(0xff0000);
CPen greenPen(PS_SOLID, 2, 0x00ff00);
CPen redPen(PS_SOLID, 2, 0x0000ff);
oldPen = pDC->GetCurrentPen();
oldBrush = pDC->GetCurrentBrush();

LocalToScreen(m_RobotPose, x_l, y_l, x_s, y_s);
Pt1.x = x_s; Pt1.y = y_s;

for(i=0;i<7;i++) // 7 sensor bars drawing from right to left
{
		

			if (m_RobotInfo.sensorValue[i] !=-1)
			{
				x_l = cos(i*PI/6-PI/2)*(ROBOT_RADIUS+m_RobotInfo.sensorValue[i]);
				y_l = sin(i*PI/6-PI/2)*(ROBOT_RADIUS+m_RobotInfo.sensorValue[i]);
				LocalToScreen(m_RobotPose, x_l, y_l, x_s, y_s);
				Pt3.x = x_s; Pt3.y = y_s;
				pDC->SelectObject(&redPen);
				pDC->MoveTo(Pt1);
				pDC->LineTo(Pt3);
				pDC->SelectObject(oldPen);
			}		
			else
			{

				x_l = 0.0f; y_l = 0.0f; //left top point
				LocalToScreen(m_RobotPose, x_l, y_l, x_s, y_s);
				Pt1.x = x_s; Pt1.y = y_s;
				x_l = (ROBOT_RADIUS+1)*cos(i*PI/6-PI/2);
				y_l = (ROBOT_RADIUS+1)*sin(i*PI/6-PI/2); //right bottom point

				LocalToScreen(m_RobotPose, x_l, y_l, x_s, y_s);
				Pt2.x = x_s; Pt2.y = y_s;
				pDC->SelectObject(&greenPen);
				pDC->MoveTo(Pt1);
				pDC->LineTo(Pt2);
				pDC->SelectObject(oldPen);

			}
		
	}	
}

void CRobot::DrawTrajectory(CDC *pDC)
{

	float x_l, y_l; //local coordinate
	int x_s, y_s;
	int i;

	CPen *oldPen;
	CPoint Pt1, Pt2;
	int PenWidth;

	GlobalToScreen(0.05f, PenWidth);//0.05m width
	CPen redPen(PS_SOLID, 2, 0x0000ff);
	oldPen = pDC->GetCurrentPen();

	x_l = 0.0f; y_l = 0.0f; //center point

	int num_trajectory=0;
	for(i=0; m_RobotTrajectory[i].x!=0; i++)	num_trajectory++;

	m_RobotTrajectory[num_trajectory].x = m_RobotPose.x;
	m_RobotTrajectory[num_trajectory].y = m_RobotPose.y;

	for(i = 0; i<num_trajectory; i++)
	{
		x_l = 0.0f; y_l = 0.0f; //left top point
		GlobalToScreen(m_RobotTrajectory[i].x, m_RobotTrajectory[i].y, x_s, y_s);
		Pt1.x = x_s; 
		Pt1.y = y_s;

		GlobalToScreen(m_RobotTrajectory[i+1].x, m_RobotTrajectory[i+1].y, x_s, y_s);
		Pt2.x = x_s; 
		Pt2.y = y_s;

		pDC->SelectObject(&redPen);
		pDC->MoveTo(Pt1);
		pDC->LineTo(Pt2);
		pDC->SelectObject(oldPen);
	}
}
