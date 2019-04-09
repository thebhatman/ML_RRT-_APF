#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
//#include <chrono> 
#include <ctime>
#include <math.h>
#include <vector>
#include <queue>

#define ROWS 200
#define COLS 300

#define GRID 12 //= GRIDX*GRIDY
#define GRIDX 3
#define GRIDY 4

using namespace cv;
using namespace std;
//using namespace std::chrono;
float max_step_size = 2.5;
float neighbour_hood = 10;

typedef struct point
{
	float x,y;
}point;

typedef struct obs_centre
{
	int i,j;
}obs_centre;

typedef struct parent_node
{
	Point_<float> curr;
	int index;
}parent_node;

typedef struct potential
{
	float magx, magy;
	float slope;
}potential;

typedef struct node
{
	point curr;
	parent_node mommy;
	int my_index;
	float cost;
	vector< parent_node > neighbours;
}node;

float dist(node p1, node p2)
{
	float d = sqrt(pow(p1.curr.x-p2.curr.x,2)+pow(p1.curr.y-p2.curr.y,2));
	return d;
}

float distance(float x1, float y1, float x2, float y2)
{
	float result = sqrt(pow(x1-x2,2)+pow(y1-y2,2));
	return result;
}

int isValid(int i, int j, Mat a)
{
	if(i < 0 || j < 0 || i >= a.cols || j >= a.rows)
		return 0;
	return 1;
}

Mat makeobs(Mat a, int i, int j, int r)
{
	int p,q;
	for(p=i-r;p<i+r;p++)
		for(q=j-sqrt(pow(r,2)-pow(p-i,2));q<j+sqrt(pow(r,2)-pow(p-i,2));q++)
			a.at<uchar>(p,q)=255;

	return a;
}

vector< node > srctree;
vector< obs_centre> obstacles;

Mat bin(Mat a)
{
	int p,q;
	for(p=0;p<a.rows;p++)
		for(q=0;q<a.cols;q++)
			if(a.at<uchar>(p,q)>127) a.at<uchar>(p,q)=255;
			else a.at<uchar>(p,q)=0;

	return a;
}

int ceiling(int num, int denom)
{
	if(num % denom == 0)
		return num/denom;
	int x = num/denom;
	return x + 1;
}

int index(node point)
{
	int row_num = point.curr.y/(ROWS/GRIDY);
	int col_num = point.curr.x/(COLS/GRIDX);
	return row_num*GRIDX + col_num;
}

int main(int argc, char** argv)
{
	float learning_parameter[GRID];
	for(int i=0; i<GRID; i++)
	{
		float n;
		scanf("%f",&n);
		learning_parameter[i] = n;
	}
    string s0 = "defensive_dataset/";
    string s1 = "img";
    int img_num;
    scanf("%d",&img_num);
    string s2;
    stringstream out;
    out << img_num;
    s2 = out.str();
    string s3 = ".jpg";
	//Mat a(500, 500, CV_8UC1, Scalar(0));
	cout<<s0+s1+s2+s3<<endl;
	Mat a = imread(s0+s1+s2+s3,0);
	a=bin(a);

	srand(time(0));
	//auto start = std::chrono::high_resolution_clock::now();
	time_t start;
	time(&start);
	int i = 0,j = 0,k = 0, curr_tree_size = 0, m, n, s;
	potential **pot_field = new potential*[a.rows];
	for(i = 0; i < a.rows; i++)
	{
		pot_field[i] = new potential[a.cols];
	}
	obs_centre hurdle;
	for(i = 0; i < a.rows; i++)
	{
		for(j = 0; j < a.cols; j++)
		{
			if(a.at<uchar>(i,j) > 200)
			{
				hurdle.i = i;
				hurdle.j = j;
				obstacles.push_back(hurdle);
			}
		}
	}
	Mat c = a.clone();
	//Mat kernel = Mat::ones(5,5, CV_8UC1);
	//dilate(c,c,kernel);
	Mat b = a.clone();
	node source, dest;
	//cout<<"Enter the co-ordinates of the source"<<endl;
	//cin>>source.curr.x>>source.curr.y;
	source.curr.x = COLS*3/4;
	source.curr.y = ROWS*1/2;
	int o = source.curr.x;
	int p = source.curr.y;
	//if(a.at<uchar>(p,o)>150) {cout<<"source is on the obstacle\n"; return 0;}
	a.at<uchar>(p,o) = 200;
	//111cout<<"Enter the co-ordinates of the destination1: "<<endl;
	//cin>>dest.curr.x>>dest.curr.y;
	dest.curr.x = 10;
	dest.curr.y = ROWS/2;
	o = dest.curr.x;
	p = dest.curr.y;
	//if(a.at<uchar>(p,o)>150) {cout<<"destination is on the obstacle\n"; return 0;}
	a.at<uchar>(p,o) = 200;
	source.mommy.curr.x = source.curr.x;
	source.mommy.curr.y = source.curr.y;
	source.mommy.index = 0;
	source.my_index = 0;
	source.cost = 0;
	node qnear, qrand;
	float d = 0, d1 = 0, mindist = 9999999, mincost = 9999999;
	Point_<float> p1,p2,p3,p4;
	parent_node temp1, temp2, temp3, temp4;
	qnear = source;
	srctree.push_back(source);
	curr_tree_size = srctree.size();
	namedWindow("star_apf",WINDOW_NORMAL);
    imshow("star_apf",a);
    while(waitKey(10)!=27){}
	while(1)
	{
		node qnew;
		if(curr_tree_size == 1)
		{
			qrand.curr.x = rand()%a.cols;
			qrand.curr.y = rand()%a.rows;
			d = dist(qnear, qrand);
			if(d > max_step_size)
			{
				qnew.curr.x = (max_step_size*qrand.curr.x + (d - max_step_size)*qnear.curr.x)/d;
				qnew.curr.y = (max_step_size*qrand.curr.y + (d - max_step_size)*qnear.curr.y)/d;
				qnew.cost = max_step_size;
			}
			else
			{
				qnew = qrand;
				qnew.cost = d;
			}
			qnew.mommy.curr.x = qnear.curr.x;
			qnew.mommy.curr.y = qnear.curr.y;
			qnew.mommy.index = 0;
			qnew.my_index = srctree.size();
			srctree.push_back(qnew);
			curr_tree_size++;
		}
		curr_tree_size = srctree.size();
		here:

		qrand.curr.x = rand()%a.cols;
		qrand.curr.y = rand()%a.rows;
		mindist = 9999999;
		int x_co, y_co;
		x_co = qrand.curr.x; y_co = qrand.curr.y;
		potential net_field;
		net_field.magx = 0;
		net_field.magy = 0;
		net_field.slope = 0;
		for(k = 0; k < obstacles.size(); k++)
		{
			float slippery_slope, angle;
			if(obstacles[k].j - qrand.curr.x != 0)
			{
				slippery_slope = 1.0*(obstacles[k].i - qrand.curr.y)/(obstacles[k].j - qrand.curr.x);
				angle = abs(atan(slippery_slope));
			}
			else
			{
				angle = 1.57;
			}
			float magnitude = learning_parameter[index(qrand)]/pow(distance(qrand.curr.x, qrand.curr.y, obstacles[k].j, obstacles[k].i),2);
			if(obstacles[k].j > qrand.curr.x && obstacles[k].i > qrand.curr.y)
			{
				net_field.magx += -magnitude*cos(angle);
				net_field.magy += -magnitude*sin(angle);
			}
			if(obstacles[k].j < qrand.curr.x && obstacles[k].i < qrand.curr.y)
			{
				net_field.magx += magnitude*cos(angle);
				net_field.magy += magnitude*sin(angle);
			}
			if(obstacles[k].j > qrand.curr.x && obstacles[k].i < qrand.curr.y)
			{
				net_field.magx += -magnitude*cos(angle);
				net_field.magy += magnitude*sin(angle);
			}
			if(obstacles[k].j > qrand.curr.x && obstacles[k].i > qrand.curr.y)
			{
				net_field.magx += magnitude*cos(angle);
				net_field.magy += -magnitude*sin(angle);
			}
			if(obstacles[k].j == qrand.curr.x && obstacles[k].i < qrand.curr.y)
			{
				net_field.magy += magnitude; 
			}
			if(obstacles[k].j == qrand.curr.x && obstacles[k].i > qrand.curr.y)
			{
				net_field.magy += -magnitude; 
			}
			if(obstacles[k].i == qrand.curr.y && obstacles[k].j > qrand.curr.x)
			{
				net_field.magx += -magnitude; 
			}
			if(obstacles[k].i == qrand.curr.y && obstacles[k].j < qrand.curr.x)
			{
				net_field.magx += magnitude; 
			}
		}
		//cout<<"Net repulsive field: "<<net_field.magx<<" "<<net_field.magy<<endl;
		float dist_dest = distance(qrand.curr.x, qrand.curr.y, dest.curr.x, dest.curr.y);
		float dest_slope, dest_angle;
		if(dest.curr.x - qrand.curr.x != 0)
		{
			dest_slope = 1.0*(dest.curr.y - qrand.curr.y)/(dest.curr.x - qrand.curr.x);
			dest_angle = abs(atan(dest_slope));
		}
		else dest_angle = 1.57;
		if(dest.curr.x > qrand.curr.x && dest.curr.y > qrand.curr.y)
		{
			net_field.magx += 1*dist_dest*dist_dest*cos(dest_angle);
			net_field.magy += 1*dist_dest*dist_dest*sin(dest_angle);
		}
		if(dest.curr.x < qrand.curr.x && dest.curr.y < qrand.curr.y)
		{
			net_field.magx += -1*dist_dest*dist_dest*cos(dest_angle);
			net_field.magy += -1*dist_dest*dist_dest*sin(dest_angle);
		}
		if(dest.curr.x > qrand.curr.x && dest.curr.y < qrand.curr.y)
		{
			net_field.magx += 1*dist_dest*dist_dest*cos(dest_angle);
			net_field.magy += -1*dist_dest*dist_dest*sin(dest_angle);
		}
		if(dest.curr.x < qrand.curr.x && dest.curr.y > qrand.curr.y)
		{
			net_field.magx += -1*dist_dest*dist_dest*cos(dest_angle);
			net_field.magy += 1*dist_dest*dist_dest*sin(dest_angle);
		}
		if(dest.curr.x == qrand.curr.x && dest.curr.y > qrand.curr.y)
		{
			net_field.magy += 1*dist_dest*dist_dest;
		}
		if(dest.curr.x == qrand.curr.x && dest.curr.y < qrand.curr.y)
		{
			net_field.magy += -1*dist_dest*dist_dest;
		}
		if(dest.curr.y == qrand.curr.y && dest.curr.x > qrand.curr.x)
		{
			net_field.magx += 1*dist_dest*dist_dest;
		}
		if(dest.curr.y == qrand.curr.y && dest.curr.x < qrand.curr.x)
		{
			net_field.magx += -1*dist_dest*dist_dest;
		}
		//cout<<"Net field: "<<net_field.magx<<" "<<net_field.magy<<endl;
		net_field.slope = abs(net_field.magy/net_field.magx);
		if(net_field.magx > 0 && net_field.magy > 0)
		{
			qrand.curr.x = qrand.curr.x + dist(qrand, dest)*cos(abs(atan(net_field.slope)));
			qrand.curr.y = qrand.curr.y + dist(qrand, dest)*sin(abs(atan(net_field.slope)));
		}
		if(net_field.magx > 0 && net_field.magy < 0)
		{
			qrand.curr.x = qrand.curr.x + dist(qrand, dest)*cos(abs(atan(net_field.slope)));
			qrand.curr.y = qrand.curr.y - dist(qrand, dest)*sin(abs(atan(net_field.slope)));
		}
		if(net_field.magx < 0 && net_field.magy > 0)
		{
			qrand.curr.x = qrand.curr.x - dist(qrand, dest)*cos(abs(atan(net_field.slope)));
			qrand.curr.y = qrand.curr.y + dist(qrand, dest)*sin(abs(atan(net_field.slope)));
		}
		if(net_field.magx < 0 && net_field.magy < 0)
		{
			qrand.curr.x = qrand.curr.x - dist(qrand, dest)*cos(abs(atan(net_field.slope)));
			qrand.curr.y = qrand.curr.y - dist(qrand, dest)*sin(abs(atan(net_field.slope)));
		}
		
		int flag2 = 0;
		if(!isValid(qrand.curr.x, qrand.curr.y, a))
		{
			qrand.curr.x = x_co;
			qrand.curr.y = y_co;
		}

		for(i = 0; i < curr_tree_size; i++)
		{
			d = dist(srctree[i], qrand);
			//d1 = dist(srctree[i], dest);
			if(d < mindist)
			{
				qnear = srctree[i];
				srctree[i].my_index = i;
				qnear.my_index = i;
				mindist = d;
			}
		}
		d = distance(qnear.curr.x, qnear.curr.y, qrand.curr.x, qrand.curr.y);
		if(d > max_step_size)
		{
			qnew.curr.x = (max_step_size*qrand.curr.x + (d - max_step_size)*qnear.curr.x)/d;
			qnew.curr.y = (max_step_size*qrand.curr.y + (d - max_step_size)*qnear.curr.y)/d;
		}
		else
		{
			qnew = qrand;
		}
		d = dist(qnear, qnew);
		for(i = 0; i < d; i++)
		{
			node q;
			q.curr.x = (i*qnew.curr.x + (d-i)*qnear.curr.x)/d;
			q.curr.y = (i*qnew.curr.y + (d-i)*qnear.curr.y)/d;
			int e = q.curr.y;
			int f = q.curr.x;
			if(a.at<uchar>(e,f) > 220) goto here;
		}
		
		for(i = 0; i < curr_tree_size; i++)
		{
			d1 = dist(qnew, srctree[i]);
			if(d1 <= neighbour_hood)
			{
				temp2.curr.x = srctree[i].curr.x; temp2.curr.y = srctree[i].curr.y;
				temp2.index = i;
				qnew.neighbours.push_back(temp2);
			}
		}
		mincost = 9999999;
		for(i = 0; i < qnew.neighbours.size(); i++)
		{
			d1 = distance(qnew.curr.x, qnew.curr.y, qnew.neighbours[i].curr.x, qnew.neighbours[i].curr.y);
			if(srctree[qnew.neighbours[i].index].cost + d1 < mincost)
			{
				mincost = srctree[qnew.neighbours[i].index].cost + d1;
				qnew.mommy.curr.x = srctree[qnew.neighbours[i].index].curr.x;
				qnew.mommy.curr.y = srctree[qnew.neighbours[i].index].curr.y;
				qnew.mommy.index = qnew.neighbours[i].index;
				qnew.cost = mincost;
			}
		}
		temp3.curr.x = qnew.curr.x; temp3.curr.y = qnew.curr.y;
		line(a, qnew.mommy.curr, temp3.curr, Scalar(128), 1, 8, 0);
		qnew.my_index = curr_tree_size;
		srctree.push_back(qnew);
		curr_tree_size++;
		/*for(i = 0; i < curr_tree_size - 1; i++)
		{
			d1 = dist(qnew, srctree[i]);
			if(d1 < neighbour_hood)
			{
				temp1.curr.x = qnew.curr.x; temp1.curr.y = qnew.curr.y;
				temp1.index = qnew.my_index;
				srctree[i].neighbours.push_back(temp1);
			}
		}*/
		int flag = 0;
		for(i = 0; i < qnew.neighbours.size(); i++)
		{
			d1 = distance(qnew.curr.x, qnew.curr.y, qnew.neighbours[i].curr.x, qnew.neighbours[i].curr.y);
			for(j = 0; j < d1; j++)
			{
				node q;
				q.curr.x = (j*qnew.curr.x + (d1-j)*qnew.neighbours[i].curr.x)/d1;
				q.curr.y = (j*qnew.curr.y + (d1-j)*qnew.neighbours[i].curr.y)/d1;
				int e = q.curr.y;
				int f = q.curr.x;
				if(a.at<uchar>(e,f) > 220) flag = 1;
			}	
			if(flag == 1) continue;	
			if(srctree[qnew.neighbours[i].index].cost > qnew.cost + d1)
			{
				temp4.curr.x = srctree[qnew.neighbours[i].index].curr.x;
				temp4.curr.y = srctree[qnew.neighbours[i].index].curr.y;
				line(a, srctree[qnew.neighbours[i].index].mommy.curr, temp4.curr, Scalar(0), 1, 8, 0);
				srctree[qnew.neighbours[i].index].mommy.curr.x = qnew.curr.x;
				srctree[qnew.neighbours[i].index].mommy.curr.y = qnew.curr.y;
				srctree[qnew.neighbours[i].index].mommy.index = qnew.my_index;
				srctree[qnew.neighbours[i].index].cost = qnew.cost + d1;
				temp3.curr.x = srctree[qnew.neighbours[i].index].curr.x;
				temp3.curr.y = srctree[qnew.neighbours[i].index].curr.y;
				line(a, srctree[qnew.neighbours[i].index].mommy.curr, temp3.curr, Scalar(128), 1, 8, 0);
			}

		}

		imshow("star_apf",a);
		waitKey(2);
		if(dist(qnew, dest) <= 2*max_step_size)
		{
			dest.mommy.curr.x = qnew.curr.x; dest.mommy.curr.y = qnew.curr.y;
			dest.mommy.index = qnew.my_index;
			dest.my_index = srctree.size() - 1;
			srctree.push_back(dest);
			temp3.curr.x = dest.curr.x; temp3.curr.y = dest.curr.y;
			line(a, dest.mommy.curr, temp3.curr, Scalar(128), 1, 8, 0);
			break;
		}

	}
	imwrite("final_tree.png",a);
    //imshow("star_apf",a);
    namedWindow("Final_apf", WINDOW_NORMAL);
	vector<node> path_points;
	node mover;
	int required_path_points = 51;
	int existing_path_points=0;
	int approx_step;
	k = srctree.size() - 1;
	while(k !=0 )
	{
		mover.curr.x = srctree[k].curr.x;
		mover.curr.y = srctree[k].curr.y;
		//path_points.push_back(mover);
		temp3.curr.x = srctree[k].curr.x;
		temp3.curr.y = srctree[k].curr.y;
		line(b, temp3.curr, srctree[k].mommy.curr, Scalar(255), 1 ,8, 0);
		k = srctree[k].mommy.index;
		existing_path_points++;
	}
	existing_path_points++;
	required_path_points+=1;
	k = srctree.size() - 1;
	int dada = k;
	while(k !=0 )
	{
		mover.curr.x = srctree[dada].curr.x;
		mover.curr.y = srctree[dada].curr.y;
		path_points.push_back(mover);
		temp3.curr.x = srctree[k].curr.x;
		temp3.curr.y = srctree[k].curr.y;
		required_path_points--;
		if((existing_path_points-1)%(required_path_points-1)==0)
			approx_step = (existing_path_points-1)/(required_path_points-1);
		else
			approx_step = (existing_path_points-1)/(required_path_points-1)+1;
		for(i = 0; i < approx_step; i++)
		{
			if(k == 0)
				break;
			k = srctree[k].mommy.index;
			existing_path_points--;
		}
		//line(b, temp3.curr, srctree[k].mommy.curr, Scalar(255), 1 ,8, 0);
		dada = k;

	}
	mover.curr.x = srctree[dada].curr.x;
	mover.curr.y = srctree[dada].curr.y;
	path_points.push_back(mover);
	//auto stop = std::chrono::high_resolution_clock::now();
	time_t stop;
	time(&stop);
	int n_num = path_points.size();
	// cout<<path_points[n-1].x<<" "<<path_points[n-1].y;
	ofstream myfile;
  	myfile.open ("apf_out.txt");
  	//myfile << "Writing this to a file.\n";
  	cout << n_num<< endl;
	for(i = n_num - 1; i >= 0; i--) 
	{
		myfile<<(int)path_points[i].curr.x<<" "<<(int)path_points[i].curr.y<<" ";
	}
	//ofs.open("test.txt", std::ofstream::out | std::ofstream::trunc);
	//cout<<endl<<n_num;
	myfile.close();
	imwrite("final_path.png",b);
	imshow("Final_apf", b);

	while(waitKey(0)!=27){}
	//cout << stop - start << endl;
	return 0;
}

// us this
// ofs.open("test.txt", std::ofstream::out | std::ofstream::trunc);