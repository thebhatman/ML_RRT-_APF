#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <queue>
#include <bits/stdc++.h>

#define ROWS 100
#define COLS 100
using namespace cv;
using namespace std;

typedef struct point
{
	int x,y;
}point;

float dist(point p1, point p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y -p2.y,2));
}

float slope(point p1, point p2)
{
	if(p1.x == p2.x) return INT_MAX;
	return ((p1.y - p2.y)*1.0)/(p1.x - p2.x);
}


int main()
{
	int i,j,k, flagx = 0, flagy = 0, fx = 0, fy = 0;
	point dest;
	dest.x = ROWS - 1;
	dest.y = COLS - 1;
	ifstream path_file("data/output.txt");
	string path_line;
	FILE *fp;
	fp = fopen("data/features.txt", "r");
	while(getline(path_file, path_line))
	{
	//getline(path_file,path_line);
		i = 2;
		//cout<<path_line[3]<<endl;
		vector<point> path_points;
		while(i < path_line.length())
		{
			point temp;
			//cout<<path_line[i]<<endl;
			flagx = 0; flagy = 0;
			if(path_line[i+1] != ',')
			{	
				temp.x = (path_line[i]-'0')*10 + path_line[i+1] - '0';
				flagx = 1;
			}
			else
			{
				temp.x = path_line[i] - '0';
			}
			if(flagx)
			{
				if(path_line[i+4] != ')')
				{
					temp.y = (path_line[i+3] - '0')*10 + path_line[i+4] - '0';
					flagy = 1;
				}
				else
				{
					temp.y = path_line[i+3] - '0';
				}
			}
			else
			{
				if(path_line[i+3] != ')')
				{
					temp.y = (path_line[i+2] - '0')*10 + path_line[i+3] - '0';
					flagy = 1;
				}
				else
				{
					temp.y = path_line[i+2] - '0';
				}
			}

			if(!flagx && !flagy)
			{
				i = i+7;
			}
			if(flagx && !flagy)
			{
				i = i+8;
			}
			if(!flagx && flagy)
			{
				i = i+8;
			}
			if(flagx && flagy)
			{
				i = i+9;
			}
			//cout<<path_line[i]<<endl;
			path_points.push_back(temp);	

		}
		point obs_loc;
		int area;
		float potential=0;
		fscanf(fp,"%d%d%d",&obs_loc.x,&obs_loc.y,&area);
		for(k = 0; k < path_points.size()-1; k++)
		{
			float dest_dist = dist(path_points[k], dest);
			float obs_dist = dist(path_points[k], obs_loc);
			float optimal_slope = slope(path_points[k],path_points[k+1]);
			float dest_angle = atan2(path_points[k].y-dest.y,path_points[k].x-dest.x);
			float obs_angle = atan2(path_points[k].y-obs_loc.y,path_points[k].x-obs_loc.x);

			//cout<<"obs_loc = "<<obs_loc.x<<" "<<obs_loc.y<<endl;
			//cout<<"path_points = "<<path_points[k].x<<" "<<path_points[k].y<<endl;
			//cout<<"dest_angle = "<<dest_angle*180/3.14159265<<endl;
			//cout<<"obs_angle = "<<obs_angle*180/3.14159265<<endl;
			if(optimal_slope*cos(obs_angle) == sin(obs_angle))
				potential += INT_MAX;
			
			else potential += pow(dest_dist,2)*pow(obs_dist,2)*((optimal_slope*cos(dest_angle))-sin(dest_angle))/((optimal_slope*cos(obs_angle)-sin(obs_angle)));

		}
		potential /= path_points.size();
		cout<<potential<<endl;
		//cout<<"obs "<<obs_loc.x<<" "<<obs_loc.y<<endl;

		//cout<<endl;
	}
	return 0;

}