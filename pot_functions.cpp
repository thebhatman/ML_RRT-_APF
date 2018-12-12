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
#define DATASET 100000
using namespace cv;
using namespace std;

typedef struct point
{
	int x,y;
}point;

typedef struct
{
	float x,y;
}POTENTIAL;

float dist(point p1, point p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y -p2.y,2));
}

float slope(point p1, point p2)
{
	if(p1.x == p2.x) return INT_MAX;
	return ((p1.y - p2.y)*1.0)/(p1.x - p2.x);
}

POTENTIAL where_is_my_pot(point obs, point on_path)
{
	POTENTIAL pot;
	float mag = 1/pow(dist(obs,on_path),2);
	float angle = atan2(on_path.y - obs.y, on_path.x - obs.x);
	pot.x = mag*cos(angle);
	pot.y = mag*sin(angle);
	//cout<<pot.x<<" ----- "<<pot.y<<endl;
	return pot;
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
	
    int number = DATASET;
    string s = "dataset/";
    string s1 = "img";
    string s3 = ".jpg";
    Mat img;
	fp = fopen("data/features.txt", "r");
	while(getline(path_file, path_line))
	{

	//getline(path_file,path_line);
		stringstream ss;
		ss<<(DATASET-number);

		number--;
		string s2 = ss.str();
		//cout<<s2<<endl;
		img = imread(s+s1+s2+s3,0);
		//cout<<"ros "<<img.rows<<" "<<img.cols<<endl;
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
			//img.at<uchar>(temp.x, temp.y) = 100;
			//cout<<path_line[i]<<endl;
			path_points.push_back(temp);	

		}
		/*namedWindow("image",WINDOW_NORMAL);
		imshow("image", img);
		while(1)
		{
			int temp = waitKey(10);
			if(temp == 27) break;
		}*/
		
		int iter;
		float FIELD = 0;
		//cout<<path_points.size()<<endl;
		for(iter=0; iter<path_points.size()-1; iter++)
		{
			int m,n;
			POTENTIAL POT;
			POT.x = 0;
			POT.y = 0;
			int obs_number = 0;
			
			for(m=0; m<ROWS; m++)
				for(n=0; n<COLS; n++)
				{
					if((int)img.at<uchar>(m,n) >= 150)
					{
						obs_number++;
						point obs;
						obs.x = m;
						obs.y = n;
						POTENTIAL temp = where_is_my_pot(obs, path_points[iter]);
						POT.x += temp.x;
						POT.y += temp.y;
					}					
				}
				POT.x /= obs_number;
				POT.y /= obs_number;
				float optimal_slope = slope(path_points[iter], path_points[iter+1]);
				POTENTIAL dest_POT;
				//cout<<POT.x<<"------"<<POT.y<<endl;
				float mag = pow(dist(dest,path_points[iter]),2);
				float angle = atan2(dest.y - path_points[iter].y, dest.x - path_points[iter].x);
				dest_POT.x = mag*cos(angle);
				dest_POT.y = mag*sin(angle);
				//cout<<"====>"<<dest_POT.y<<endl;
				//cout<<"BOOO "<<(optimal_slope*dest_POT.x)<<endl;
				if(optimal_slope*POT.x == POT.y)
					FIELD += INT_MAX;
				else
				FIELD += (dest_POT.y-(optimal_slope*dest_POT.x))/((optimal_slope*POT.x)-POT.y);
			//cout<<"Num = "<<(dest_POT.y-(optimal_slope*dest_POT.x));
			//cout<<"Denom = "<<((optimal_slope*POT.x)-POT.y);
			//cout<<"testing "<<FIELD<<endl;
		}

		FIELD /= path_points.size();
		cout<<FIELD<<endl;
	}
	
	return 0;

}