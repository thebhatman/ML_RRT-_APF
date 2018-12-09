#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <bits/stdc++.h>
 
using namespace cv;
using namespace std;

int index_of_best_node(int *visited, vector<Point2d> &distinct_corners, vector<float> &dist, Mat img)
{
	int i,j,k;
	int n = distinct_corners.size();
	int  min_dist = INT_MAX;
	j = -1;
	for(i = 0; i < n; i++)
	{
		if(dist[i] < min_dist && visited[i] == 0 && img.at<uchar>(distinct_corners[i].y, distinct_corners[i].x) < 127)
		{
			min_dist = dist[i];
			j = i;
		}
	}
	return j;
}


void dijkstra(vector<Point2d> all_points, Mat img, int *visited ,vector<int> &baap_ka_index)
{
	int i,j,k;
	int start_index = 0;
	int n = img.rows*img.cols;
	int end_index = n-1;
	vector<float> dist(n);
	for(i = 0; i < n; i++)
	{
		visited[i] = 0;
		if(i != start_index)
		{
			dist[i] = INT_MAX;
		}
		else
			dist[i] = 0;
		baap_ka_index[i] = -1;
	}
	int curr_node_index;
	float temp_dist;
	while(1)
	{
		//cout<<"Dijkstra running..."<<endl;
		if(curr_node_index == end_index) break; 
		if(curr_node_index == -1) continue;
		curr_node_index = index_of_best_node(visited, all_points, dist, img);
		//if(visited[curr_node_index] == 1) continue;
		//cout<<"curr_node_index = "<<curr_node_index<<endl;
		//cout<<"curr_node "<<distinct_corners[curr_node_index].x<<" "<<distinct_corners[curr_node_index].y<<endl;
		//cout<<path_matrix[curr_node_index][0]<<endl;
		if(dist[curr_node_index] + 1 < dist[curr_node_index + 1] && img.at<uchar>(all_points[curr_node_index+1].y, all_points[curr_node_index+1].x) < 127)
		{
			dist[curr_node_index+1] = dist[curr_node_index] + 1;
			baap_ka_index[curr_node_index+1] = curr_node_index;
		}
		if(dist[curr_node_index] + 1 < dist[curr_node_index - 1] && img.at<uchar>(all_points[curr_node_index-1].y, all_points[curr_node_index-1].x) < 127)
		{
			dist[curr_node_index-1] = dist[curr_node_index] + 1;
			baap_ka_index[curr_node_index-1] = curr_node_index;
		}
		if(dist[curr_node_index] + 1 < dist[curr_node_index - img.cols] && img.at<uchar>(all_points[curr_node_index-img.cols].y , all_points[curr_node_index-img.cols].x) < 127)
		{
			dist[curr_node_index-img.cols] = dist[curr_node_index] + 1;
			baap_ka_index[curr_node_index-img.cols] = curr_node_index;
		}
		if(dist[curr_node_index] + 1 < dist[curr_node_index + img.cols] && img.at<uchar>(all_points[curr_node_index+img.cols].y, all_points[curr_node_index+img.cols].x) <127)
		{
			dist[curr_node_index+img.cols] = dist[curr_node_index] + 1;
			baap_ka_index[curr_node_index+img.cols] = curr_node_index;
		}		//cout<<baap_ka_index[j]<<endl;
		visited[curr_node_index] = 1;
	}
	cout<<"Hello"<<endl;   //Just like that :P

}

void draw_final_path(vector<Point2d> &distinct_corners, Mat b, vector<int> &baap_ka_index)
{
	int start_index = 0;
	int end_index = distinct_corners.size() - 1;
	Point2d curr_node;
	curr_node.x = distinct_corners[end_index].x;
	curr_node.y = distinct_corners[end_index].y;
	//graph.push_back(curr_node);
	Point2d baap_node;
	int curr_index = end_index;
	int baap_index = baap_ka_index[curr_index];
	while(1)
	{
		if(curr_node.x == distinct_corners[start_index].x && curr_node.y == distinct_corners[start_index].y)
		{
			break;
		}
		baap_node.x = distinct_corners[baap_index].x;
		baap_node.y = distinct_corners[baap_index].y;
		Point2d start, end;
		start.x = curr_node.y;
		start.y = curr_node.x;
		end.x = baap_node.y;
		end.y = baap_node.x;
		line(b, start, end, Scalar(255), 2 , 8);
		curr_node = baap_node;
		curr_index = baap_index;
		baap_index = baap_ka_index[curr_index];
		//graph.push_back(curr_node);		
	}
}


int main()
{
	Mat img = imread("img0.jpg", 0);
	vector<Point2d> all_points;
	int i,j,k;
	int *visited = new int[img.rows*img.cols];
	vector<int> baap_ka_index(img.rows*img.cols);
	for(i = 0; i < img.rows; i++)
	{
		for(j = 0; j < img.cols; j++)
		{
			Point2d newpoint;
			newpoint.x = j;
			newpoint.y = i;
			all_points.push_back(newpoint);
		}
	}
	dijkstra(all_points, img, visited, baap_ka_index);
	draw_final_path(all_points, img, baap_ka_index);
	namedWindow("path", WINDOW_NORMAL);
	imshow("path", img);
	while(1)
	{
		int flag = waitKey(10);
		if(flag == 27)
			break;
	}
	return 0;

}