#pragma once

#define METER_PER_PIXEL 0.01f
#define Round(fX) (fX>=0)? (int)(fX+0.5):(-1)*(int)(0.5-fX)
#define PI		3.141592f
#include <stack>

typedef struct{
	float x;
	float y;
	float theta;
}POSE;

struct node{
	int next_x,next_y;
	int map_x,map_y;
	int visited;
	float local_cost; //f(n)
};
struct ROBOTINFO{
	POSE pose;
	float v;
	float w;
	float sensorValue[7];
	std::stack<node> path_stack;
	//you can add any other information you want
};


void SetParameter(int nMapWidth, int nMapHeight, int nScreenWidth, int nScreenHeight);
void LocalToGlobal(POSE referencePose, float x_l, float y_l, float &x_g, float &y_g);
void GlobalToMap(float l_g, int &l_m);
void GlobalToMap(float x_g, float y_g, int &x_m, int &y_m);
void MapToScreen(int l_m, int &l_s);
void MapToScreen(int x_m, int y_m, int &x_s, int &y_s);
void GlobalToScreen(float l_g, int &l_s);
void GlobalToScreen(float x_g, float y_g, int &x_s, int &y_s);
void LocalToScreen(POSE referencePose, float x_l, float y_l, int &x_s, int &y_s);
