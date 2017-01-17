#include "StdAfx.h"
#include "RobotMath.h"
#include <math.h>

int gnMapWidth, gnMapHeight, gnScreenWidth, gnScreenHeight;
float gfScreenPerMapRatio;
float gfCoordinateOffsetX;
float gfCoordinateOffsetY;

void SetParameter(int nMapWidth, int nMapHeight, int nScreenWidth, int nScreenHeight)
{
	gnMapWidth = nMapWidth;
	gnMapHeight = nMapHeight;
	gnScreenWidth = nScreenWidth;
	gnScreenHeight = nScreenHeight;
	gfScreenPerMapRatio = (float)gnScreenWidth/gnMapWidth;
	gfCoordinateOffsetX = (float)gnMapWidth*METER_PER_PIXEL/2.0f;
	gfCoordinateOffsetY = (float)gnMapHeight*METER_PER_PIXEL/2.0f;
}

void LocalToGlobal(POSE referencePose, float x_l, float y_l, float &x_g, float &y_g)
{
	x_g = (float)(cos(referencePose.theta)*x_l - sin(referencePose.theta)*y_l + referencePose.x);
	y_g = (float)(sin(referencePose.theta)*x_l + cos(referencePose.theta)*y_l + referencePose.y);
}

void GlobalToMap(float l_g, int &l_m)
{
	l_m = Round(l_g/METER_PER_PIXEL);	
}

void GlobalToMap(float x_g, float y_g, int &x_m, int &y_m)
{
	x_m = Round((x_g + gfCoordinateOffsetX)/METER_PER_PIXEL);
	y_m = Round(gnMapHeight - (y_g + gfCoordinateOffsetY)/METER_PER_PIXEL);
}

void MapToScreen(int l_m, int &l_s)
{
	l_s = Round(gfScreenPerMapRatio*l_m);
}

void MapToScreen(int x_m, int y_m, int &x_s, int &y_s)
{
	x_s = Round(gfScreenPerMapRatio*x_m);
	y_s = Round(gfScreenPerMapRatio*y_m);
}

void GlobalToScreen(float l_g, int &l_s)
{
	int l_m;
	GlobalToMap(l_g, l_m);
	MapToScreen(l_m, l_s);
}

void GlobalToScreen(float x_g, float y_g, int &x_s, int &y_s)
{
	int x_m, y_m;
	GlobalToMap(x_g, y_g, x_m, y_m);
	MapToScreen(x_m, y_m, x_s, y_s);
}

void LocalToScreen(POSE referencePose, float x_l, float y_l, int &x_s, int &y_s)
{
	float x_g, y_g;
	LocalToGlobal(referencePose, x_l, y_l, x_g, y_g);
	GlobalToScreen(x_g, y_g, x_s, y_s);
}