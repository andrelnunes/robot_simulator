#pragma once
#include "OpenCV/cv.h"
#include "OpenCV/highgui.h"
#include "OpenCV/cxcore.h"
#include "RobotMath.h"

void InitializeStartGoalNavigation(IplImage* pGlobalMap, ROBOTINFO &robotInfo);
bool RunStartGoalNavigation(IplImage* pGlobalMap, ROBOTINFO &robotInfo, float *v_target, float *w_target);
