#include "StdAfx.h"
#include "Algorithm.h"
#include <math.h>
#include <stack>

const int n= 100; // horizontal size of the map
const int m= 100; // vertical size size of the map
static int dx[8]={1, 1, 0, -1, -1, -1, 0, 1};
static int dy[8]={0, 1, 1, 1, 0, -1, -1, -1};
static int closed_nodes_map[n][m]; // map of closed (tried-out) nodes
static int open_nodes_map[n][m]; // map of open (not-yet-tried) nodes
static float distance_map[n][m]; //h(n)
static float current_cost_map[n][m]; //g(n)
static int last_known_x;
static int last_known_y;

/*struct node{
	int next_x,next_y;
	int map_x,map_y;
	int visited;
	float local_cost; //f(n)
};*/

static node open_nodes[n][m];

void InitializeStartGoalNavigation(IplImage* pGlobalMap, ROBOTINFO &robotInfo)
{
	IplImage *map = cvCreateImage( cvGetSize(pGlobalMap), IPL_DEPTH_8U, 1);
	cvCopyImage(pGlobalMap, map);
	std::stack<node> mystack;
	std::stack<node> tempstack;
	node current_node;

	for( int y1 = 0 ; y1 < m ; y1++)
	{
		for (int x1 = 0 ; x1 < n; x1++)
		{
			closed_nodes_map[y1][x1] = 0; 
			open_nodes_map[y1][x1] = 0;
			open_nodes[y1][x1].visited = 0;
			open_nodes[y1][x1].local_cost = 1000000;
			distance_map[y1][x1] = 1000000;
			current_cost_map[y1][x1] = 1000000;
		}
	}

	for( int y=0 ; y < map->height ; y++)
	{
		for( int x=0 ; x < map->width ; x++)
		{
			int k = cvGetReal2D(map, y, x);

			if(k == 0)
			{
				for( int j = -3 ; j < 3 ; j++)
				{
					for ( int i = -3 ; i < 3 ; i++)
					{
						if ( (x/10 +i >= 0 ) && ((x/10) +i <= m) && ( (y/10) +j >= 0 ) && ( (y/10)+j <= n ))
						{
							if ( closed_nodes_map[(int) (y/10) +j][(int) (x/10 ) + i] == 0 )
							{
								closed_nodes_map[(int) (y/10) +j ][(int) (x/10) + i] = 1;
							}
						}
					}
				}

				for (int temp1 = -30; temp1 <= 40; temp1++)
				{
					for(int temp2 = -30; temp2 <= 40; temp2++)
					{
						if ( (x+temp2 >= 0 ) && (x+temp2 <= map->width) && (y+temp1 >= 0 ) && (y+temp1 <= map->height ))
						{
							if ((closed_nodes_map[(int) ((y+temp1)/10)][(int) ((x+temp2)/10)]) == 1)
							{
								if(map->imageData[(y+temp1)*(map->width) + (x+temp2)] != 0)
								{
									map->imageData[(y+temp1)*(map->width) + (x+temp2)] = 50;
								}
							}
						}
					}
				}
			}
		}
	}

	int map_x = 90,map_y = 10,temp_x,temp_y;
	open_nodes_map[map_y][map_x] = 1;
	open_nodes[map_y][map_x].visited = 1;
	open_nodes[map_y][map_x].map_x = map_x;
	open_nodes[map_y][map_x].map_y = map_y;
	current_cost_map[map_y][map_x] = 0;
	mystack.push(open_nodes[map_y][map_x]);

	while( !(( map_x == 10 ) && ( map_y == 90 )))
	{
		float temp_cost = 1000000;
		temp_x = open_nodes[map_y][map_x].map_x;
		temp_y = open_nodes[map_y][map_x].map_y;
		current_node = mystack.top();
		mystack.pop();

		for ( int i = 0 ; i < 8 ; i++)
		{
			open_nodes[map_y + dy[i]][map_x + dx[i]].map_x = map_x + dx[i];
			open_nodes[map_y + dy[i]][map_x + dx[i]].map_y = map_y + dy[i];
			
			open_nodes_map[map_y + dy[i]][map_x + dx[i]] = 1;
			
			distance_map[map_y + dy[i]][map_x + dx[i]] = sqrt( (float) ((map_x + dx[i] - 10)*(map_x + dx[i] - 10) + (map_y + dy[i] - 90)*(map_y + dy[i] - 90)) );

			//current_cost_map[map_y + dy[i]][map_x + dx[i]] = min( current_cost_map[map_y + dy[i]][map_x + dx[i]], current_cost_map[map_y][map_x] + sqrt( (float)( dx[i]*dx[i] + dy[i]*dy[i] ) ) );		
			
			if(current_cost_map[map_y + dy[i]][map_x + dx[i]] != min(current_cost_map[map_y + dy[i]][map_x + dx[i]], current_cost_map[map_y][map_x] +  sqrt( (float) (dx[i])*(dx[i]) + (dy[i])*(dy[i]) )) )
			{
				open_nodes[map_y + dy[i]][map_x + dx[i]].next_x = map_x;
				open_nodes[map_y + dy[i]][map_x + dx[i]].next_y = map_y;
				current_cost_map[map_y +dy[i]][map_x + dx[i]] = current_cost_map[map_y][map_x] +  sqrt( (float) (dx[i])*(dx[i]) + (dy[i])*(dy[i]) );
			}

			open_nodes[map_y + dy[i]][map_x + dx[i]].local_cost = 2*distance_map[map_y + dy[i]][map_x + dx[i]] + current_cost_map[map_y + dy[i]][map_x + dx[i]];

			if( (open_nodes[map_y + dy[i]][map_x + dx[i]].local_cost < temp_cost) && (open_nodes[map_y + dy[i]][map_x + dx[i]].visited == 0) )
			{
				temp_cost = open_nodes[map_y + dy[i]][map_x + dx[i]].local_cost;
				temp_x = map_x + dx[i];
				temp_y = map_y + dy[i];			
			}
		}
		
		if(closed_nodes_map[temp_y][temp_x] == 1)
		{
			int map_x2 = map_x;
			int map_y2 = map_y;
			int choice2_x, choice2_y;
			float temp_cost_2 = 1000000, temp_cost = 100000;

			for(int i = 0; i<8; i++)
			{
				if( (open_nodes[map_y2 + dy[i]][map_x2 + dx[i]].local_cost < temp_cost) && (closed_nodes_map[map_y2 + dy[i]][map_x2 + dx[i]] == 0) && (open_nodes[map_y2 + dy[i]][map_x2 + dx[i]].visited == 0) )
				{
					temp_cost = open_nodes[map_y2 + dy[i]][map_x2 + dx[i]].local_cost;
					temp_x = map_x2 + dx[i];
					temp_y = map_y2 + dy[i];			
				}
			}

			int flag = 0;
			for(int i = 0; i<8; i++)
			{
				if( (open_nodes[map_y2 + dy[i]][map_x2 + dx[i]].local_cost < temp_cost_2) && (closed_nodes_map[map_y + dy[i]][map_x + dx[i]] == 0) && !( (map_y + dy[i] == temp_y) && (map_x + dx[i] == temp_x) ) )
				{
					temp_cost_2 = open_nodes[map_y2 + dy[i]][map_x2 + dx[i]].local_cost;
					choice2_x = map_x2 + dx[i];
					choice2_y = map_y2 + dy[i];			
					flag = 1;
				}
			}

			if(flag = 1)
			{
				mystack.push(open_nodes[choice2_y][choice2_x]);
			}
	}
		
		
		open_nodes[map_y][map_x].visited= 1;

		//map->imageData[map_y*10*map->width + map_x*10] = 150;
		//cvCircle(map, cvPoint(map_x*10,map_y*10), 2, cvScalar(0,255,0) ,-1);

		map_x = temp_x;
		map_y = temp_y;

		current_node = open_nodes[map_y][map_x];

		while( !(mystack.empty()) )
		{
			if ( (mystack.top().local_cost) < (current_node.local_cost) )
			{
				tempstack.push(current_node);
				current_node = mystack.top();
				mystack.pop();
			}
			else
			{
				break;
			}
		}

		mystack.push(current_node);
		while( !(tempstack.empty()) )
		{
			mystack.push(tempstack.top());
			tempstack.pop();
		}

		map_x = mystack.top().map_x;
		map_y = mystack.top().map_y;


	}

	//Storing of nodes
	int current_cost = 10000;
	int map_y1 = map_y, map_x1 = map_x;
	tempstack.push(open_nodes[map_y1][map_x1]);

	while(!(( tempstack.top().map_x == 90 ) && ( tempstack.top().map_y == 10 )))
	{
		tempstack.push(open_nodes[open_nodes[map_y1][map_x1].next_y][open_nodes[map_y1][map_x1].next_x]);
		map_y1 = tempstack.top().next_y;
		map_x1 = tempstack.top().next_x;
		map->imageData[map_y1*10*map->width + map_x1*10] = 150;
		cvCircle(map, cvPoint(map_x1*10,map_y1*10), 2, cvScalar(0,255,0) ,-1);
	}
	while( !(tempstack.empty()) )
	{
		robotInfo.path_stack.push(tempstack.top());
		tempstack.pop();
	}

	last_known_x = map_x;
	last_known_y = map_y;

	cvNamedWindow( " PathPlanning ", 1/2);
	cvShowImage( " PathPlanning ", map);
	cvReleaseImage(&map);
	
}

bool RunStartGoalNavigation(IplImage* pGlobalMap, ROBOTINFO &robotInfo, float *v_target, float *w_target)
{
	float temp_cost = 10000;
	int target_x = last_known_x, target_y = last_known_y, map_x, map_y;//, temp_x, temp_y, flag = 0;
	map_x = (int)(robotInfo.pose.x*10) + 50;
	map_y = 50 - (int)(robotInfo.pose.y*10);

	/*if( (robotInfo.pose.x == 4.0f) && (robotInfo.pose.y == 4.0f) )
	{
		*v_target = 0.0;
		*w_target = 0.0;
		return false;
	} */
	if( (abs(robotInfo.pose.x - 4.0f) <= 0.1f) && (abs(robotInfo.pose.y - 4.0f) <= 0.1f) )
	{
		target_x = 90;
		target_y = 10;
		*v_target = 0.0;
		*w_target = 0.0;
		return false;
	}
		
	if( ( map_x == robotInfo.path_stack.top().next_x) && (map_y == robotInfo.path_stack.top().next_y) )
	{
			robotInfo.path_stack.pop();
	}
		
	target_x = robotInfo.path_stack.top().next_x;
	target_y = robotInfo.path_stack.top().next_y;

	float direction = atan2((float) -(target_y - map_y), target_x - map_x ) - robotInfo.pose.theta;

	if (abs(direction) > 0.3f)
	{
		if (direction > 0)
		{
			*w_target = 0.3f;
		}
		else
		{
			*w_target = -0.3f;
		}

		*v_target = 0.0f;
	}
	else
	{
		*w_target = direction;
		*v_target = 0.2f;
	}
	
	if( direction == 0 )
	{
		*v_target = 0.3f;
	}

	last_known_x = target_x;
	last_known_y = target_y;
	
	return TRUE;
}


