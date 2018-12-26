#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <pthread.h>

#include <iostream>
#include <time.h>
#include <vector>

#include <stdio.h>
#include<math.h>
#include <stdlib.h>
#include <unistd.h>

using namespace cv;
using namespace std;

float learning_parameter = 1000;

struct parent
{
	Point2i point;
	int index;
};

struct node
{
	Point2i point;
	int index;
	float cost;
	struct parent dad;
	Vector<struct parent> neighbors;
};

Mat bin(Mat a)
{
	int p,q;
	for(p=0;p<a.rows;p++)
		for(q=0;q<a.cols;q++)
		{
			if(a.at<uchar>(p,q)<127) a.at<uchar>(p,q)=0;
			else a.at<uchar>(p,q)=255;
		}

		return a;
}

float d(Point2i p1, Point2i p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y- p2.y, 2));
}

Point2i step(Point2i p1, Point2i p2, float section)
{
	float dist = d(p1,p2);
	if(dist <= section) return p2;

	Point2i p;
	p.x = (int)(((dist-section)*p1.x) + (section*p2.x))/dist;
	p.y = (int)(((dist-section)*p1.y) + (section*p2.y))/dist;

	return p;
}

int findi(Point2i p, Vector<Point2i> v)
{
	int i;
	int index=-1;
	for(i=0;i<v.size();i++)
	{
		if(v[i].x==p.x && v[i].y==p.y) {index=i; break;}
	}

	return index;
}

Point2i field_effect(Mat a, Point2i q_rand, Vector<Point2i> obstacles, Point2i end)
{
	float END_ZONE = (a.rows+a.cols)/20;
	float dist=sqrt(pow(end.x-q_rand.x,2)+pow(end.y-q_rand.y,2));
	if(dist<END_ZONE) return q_rand;

	float F_x=0, F_y=0, F=0;
	int i;
	for(i=0;i<obstacles.size();i++)
	{
		float force = 1/(pow(obstacles[i].x - q_rand.x, 2)+pow(obstacles[i].y - q_rand.y, 2));
		F_x += force*cos(atan2(obstacles[i].y - q_rand.y, obstacles[i].x - q_rand.x));
		F_y += force*sin(atan2(obstacles[i].y - q_rand.y, obstacles[i].x - q_rand.x));
	}
	F_x *= -1*learning_parameter;
	F_y *= -1*learning_parameter;

	float force = (pow(end.x - q_rand.x, 2) + pow(end.y - q_rand.y, 2));
	F_x += force*cos(atan2(end.y - q_rand.y, end.x - q_rand.x));
	F_y += force*sin(atan2(end.y - q_rand.y, end.x - q_rand.x));

	F = sqrt(pow(F_x, 2) + pow(F_y, 2));
	q_rand.x += 2*END_ZONE*F_x/F;
	q_rand.y += 2*END_ZONE*F_y/F;

	return q_rand;

}

int isValid(Mat a, int i, int j)
{
	if(i<0 || i>=a.rows || j<0 || j>=a.cols) return 0;
	return 1;
}

int main()
{

	//printf("Instructions:\nIn the beginning the image will be frozen.\nThe dim spot is the source.\nThe two bright spots are the destinations.\nPress 'Esc' to start.\nThe final path is displayed in a new window at the end, named 'Final'.\nPress 'Esc' at the end to stop code.\n");
	srand(time(0));

	//a=imread("Untitled1.png",0);
	//a=bin(a);

	Mat a = imread("dataset/img99999.jpg",0);
	a = bin(a);

	namedWindow("Image",WINDOW_NORMAL);
	imshow("Image",a);
	while(waitKey(0)!=27){}	

	float step_size = 3;
	float neighborhood = step_size;
	
	Vector<struct node> tree;
	
	int flag = 0;

	int **indices = (int **)malloc(a.rows*sizeof(int *));
	int iter;
	for(iter=0; iter<a.rows; iter++)
		indices[iter] = (int *)malloc(a.cols*sizeof(int));

	struct node src, end;

	src.point.y = 0;
	src.point.x = 0;
	
	src.index = 0;
	indices[src.point.y][src.point.x] = 0;
	src.cost = 0;
	src.dad.point = src.point;
	src.dad.index = 0;
	a.at<uchar>(src.point.y,src.point.x)=128;	
	tree.push_back(src);

	end.point.y = a.rows-1;
	end.point.x = a.cols-1;
	//a.at<uchar>(end.point.y,end.point.x)=254;

	//imshow("Image",a);
	//while(waitKey(0)!=27){}

	int K=0, p, q;
	Vector<Point2i> obstacles;
	for(p=0; p<a.rows; p++)
		for(q=0; q<a.cols; q++)
		{
			if(a.at<uchar>(p,q) == 0) K++;
			else
			{
				Point2i temp;
				temp.y = p;
				temp.x = q;
				obstacles.push_back(temp);
			}

		}

	int count = 1;
	while(count++ != K)
		{
			//printf("start of while loop\n");
			struct node q_rand;
			do
			{
			q_rand.point.y = rand()%a.rows;
			q_rand.point.x = rand()%a.cols;
			q_rand.point = field_effect(a, q_rand.point, obstacles, end.point);
			}while(!isValid(a, q_rand.point.y, q_rand.point.x) || a.at<uchar>(q_rand.point.y, q_rand.point.x) == 255);
			//printf("q_rand generated\n");

			float minimum = d(src.point, q_rand.point);
			struct node q_nearest = src;

			int i;
			for(i=1; i<tree.size(); i++)
			{	
				float dist = d(tree[i].point, q_rand.point);
				if(dist < minimum) {minimum = dist; q_nearest = tree[i];}
			}
			//printf("q_nearest found\n");

			int obs, section;
			for(section=1, obs=0; section<minimum; section++)
			{	
				Point2i check;
				check = step(q_rand.point, q_nearest.point, section);
				if(a.at<uchar>(check.y, check.x) == 255) {obs=1; break;}
			}

			if(obs) continue;
			//printf("no obstacle in between\n");

			struct node q_new;
			q_new.point = step(q_nearest.point, q_rand.point, step_size);
			
			q_new.dad.point = q_nearest.point;
			q_new.dad.index = q_nearest.index;
			q_new.cost = q_nearest.cost + d(q_nearest.point, q_new.point);

			/*for(i=1; i<tree.size(); i++)
			{
				if(d(q_new.point,tree[i].point) < neighborhood)
				{
					struct parent temp;
					temp.point=tree[i].point;
					temp.index=i;
					q_new.neighbors.push_back(temp);
				}
			}
			
			for(i=0; i<cur.neighbors.size(); i++)
			{
				if(tree[cur.neighbors[i].index].cost + d(cur.neighbors[i].point,cur.point) < cur.cost)
				{
					cur.dad = cur.neighbors[i];
					cur.cost = tree[cur.neighbors[i].index].cost + d(cur.neighbors[i].point,cur.point);
				} 
			}*/

			for(p=q_new.point.y-neighborhood; p<=q_new.point.y+neighborhood; p++)
				for(q=q_new.point.x-neighborhood; q<=q_new.point.x+neighborhood; q++)
					if(isValid(a,p,q) && a.at<uchar>(p,q)==128)
						if(tree[indices[p][q]].cost + sqrt(pow(p - q_new.point.y, 2) + pow(q - q_new.point.x, 2)) < q_new.cost)
						{	
							q_new.dad.point.y = p;
							q_new.dad.point.x = q;
							q_new.dad.index = indices[p][q];
							//printf("special dad index = %d\n",indices[p][q]);
							q_new.cost = tree[q_new.dad.index].cost + d(q_new.dad.point,q_new.point);
						}


			q_new.index = tree.size();
			indices[q_new.point.y][q_new.point.x] = tree.size();
			tree.push_back(q_new);
			//printf("dad of q_new just added = %d\n",tree[tree.size()-1].dad.index);
			line(a,q_new.point,q_new.dad.point,Scalar(127),1,8,0);
			a.at<uchar>(q_new.point.y,q_new.point.x)=128;
			a.at<uchar>(q_new.dad.point.y,q_new.dad.point.x)=128;
			//printf("q_new added\n");

			/*for(i=0; i<cur.neighbors.size(); i++)
			{
				if(tree[cur.neighbors[i].index].cost > cur.cost + d(cur.point,cur.neighbors[i].point))
				{	
					line(a,cur.neighbors[i].point,tree[cur.neighbors[i].index].dad.point,Scalar(0),1,8,0);					
					struct parent temp;
					temp.point=cur.point;
					temp.index=cur.index;
					tree[cur.neighbors[i].index].dad=temp;
					tree[cur.neighbors[i].index].cost = cur.cost + d(cur.point,cur.neighbors[i].point);
					line(a,cur.neighbors[i].point,tree[cur.neighbors[i].index].dad.point,Scalar(127),1,8,0);
				} 
			}*/

			for(p=q_new.point.y-neighborhood; p<=q_new.point.y+neighborhood; p++)
				for(q=q_new.point.x-neighborhood; q<=q_new.point.x+neighborhood; q++)
					if(isValid(a,p,q) && a.at<uchar>(p,q)==128)
						if(tree[indices[p][q]].cost > q_new.cost + sqrt(pow(p - q_new.point.y, 2) + pow(q - q_new.point.x, 2)))
						{
							line(a, tree[indices[p][q]].point, tree[indices[p][q]].dad.point, Scalar(0), 1, 8, 0);
							tree[indices[p][q]].dad.point = q_new.point;
							tree[indices[p][q]].dad.index = q_new.index;
							//printf("a neighbor's dad just rewired to = %d\n",tree[indices[p][q]].dad.index);
							tree[indices[p][q]].cost = q_new.cost + sqrt(pow(p - q_new.point.y, 2) + pow(q - q_new.point.x, 2));
							line(a, tree[indices[p][q]].point, q_new.point, Scalar(127), 1, 8, 0);
							a.at<uchar>(p,q) = 128;
							a.at<uchar>(q_new.point.y, q_new.point.x) = 128;
						}
			//printf("neighbors rewired\n");
			
			imshow("Image",a);
			waitKey(1);
			//while(waitKey(0)!=27){}

			if(d(q_new.point,end.point) <= 2*neighborhood) 
				{	
					//printf("reached end\n");
					end.dad.point = q_new.point;
					end.dad.index = q_new.index;
					end.cost = q_new.cost + d(q_new.point, end.point);
					end.index = tree.size();
					//printf("end.index = %d, end.dad.index = %d\n",end.index,end.dad.index);
					indices[end.point.y][end.point.x] = tree.size();
					tree.push_back(end);
					flag = 1;
					line(a, q_new.point, end.point, Scalar(127), 1, 8, 0);
					a.at<uchar>(q_new.point.y, q_new.point.x) = 128;
					a.at<uchar>(end.point.y, end.point.x) = 128;
					imshow("Image",a);
					waitKey(1);
					break;
				}
			//printf("at end of while loop\n");

		}

	if(flag==0) {printf("Can't reach any destination\n"); imshow("Image",a);	while(waitKey(0)!=27){} return 0;}
	
	Mat path(a.rows, a.cols, CV_8UC3, Scalar(0,0,0));
	path.at<Vec3b>(src.point.y,src.point.x)[1] = 255;
	path.at<Vec3b>(end.point.y,end.point.x)[2] = 255;
	
	namedWindow("Final",WINDOW_NORMAL);

	//printf("tree.size = %d\n",(int)tree.size());
	int l = tree.size()-1;

	//int m;
	//for(m=0; m<=l; m++)
	//	printf("dad of %d = %d\n",m,tree[m].dad.index);

		
	while(l!=0)
	{	
		
		line(path, tree[l].point, tree[l].dad.point, Scalar(255,255,255), 1, 8, 0);
		//printf("first:- %d,%d\n",temp.point.x,temp.point.y);
		//printf("second:- %d,%d\n",tree[l].dad.point.x,tree[l].dad.point.y);
		l = tree[l].dad.index;
		//printf("l = %d\n",l);
		//printf("%d\n",++testing);
	}

	imwrite("Image.png", a);
	imwrite("Final.png", path);
	imshow("Image", a);
	imshow("Final", path);
	while(waitKey(0)!=27){}

	printf("Done\n");

	return 0;
}
