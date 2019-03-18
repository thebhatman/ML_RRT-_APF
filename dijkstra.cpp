#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
using namespace cv;
#define ROWS 200
#define COLS 300
#define DATASET 10
Mat img;
struct graph_node
{
	int x,y;
};
int isvalid( int i, int j)
{
	//cout<<"IN isvalid"<<i<<" "<<j<<endl;
	if(i>=0 && j>=0 && i<ROWS && j<COLS)
	{
		if(img.at<uchar>(i,j)<100)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}
graph_node minDistance(float dist[ROWS][COLS], int sptSet[ROWS][COLS]) 
{ 
   // Initialize min value 
   float min = INT_MAX; 
   graph_node node;
   for(int i=0;i<ROWS;i+=10)
   		for(int j=0;j<COLS;j+=10)
     		if (sptSet[i][j] == 0 && dist[i][j] <= min)
     		{ 
         		min = dist[i][j];
         		node.x=i;
         		node.y=j;
         	} 
   return node; 
} 
int printSolution(float dist[ROWS][COLS]) 
{ 
	printf("Vertex   Distance from Source\n"); 
	for(int i=0;i<ROWS;i++)
		for(int j=0;j<COLS;j++) 
			printf("(%d,%d)->%f\n", i,j,dist[i][j]); 
} 
void dijkstra(graph_node start, graph_node end, graph_node parent[ROWS][COLS]) 
{ 
	//cout<<"in dijkstra"<<endl;
	float dist[ROWS][COLS];
	int sptSet[ROWS][COLS];
	for(int i = 0; i < ROWS; i++) 
		for(int j=0;j<COLS;j++)
		{
			dist[i][j] = INT_MAX;
			sptSet[i][j] = 0;  
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
	dist[start.x][start.y] = 0;
	for (int count = 0; count < ROWS*COLS-1; count++) 
	{  
		//cout<<count<<endl;
		graph_node node = minDistance(dist, sptSet); 
		//cout<<node.x<<"----"<<node.y<<endl;
		int X=node.x;
		int Y=node.y;
		//cout<<"ISVALID VALUES:: "<<isvalid(X-1,Y)<<" "<<isvalid(X,Y-1)<<" "<<isvalid(X,Y+1)<<" "<<isvalid(X+1,Y)<<endl;
		//cout<<"k";
		if(node.x==end.x && node.y==end.y)
		{
			//cout<<"breaking";
			break; 
		}
		sptSet[node.x][node.y] = 1;
		//top 
		if (isvalid(X-10,Y) && !sptSet[X-10][Y] && (dist[X][Y]+10 < dist[X-10][Y]))
		{ 
			//cout<<"TRUE1"<<endl;
		    dist[X-10][Y] = dist[X][Y]+10;
		    parent[X-10][Y].x=X;
		    parent[X-10][Y].y=Y;
		}
		//top-left
		if (isvalid(X-10,Y-10) && !sptSet[X-10][Y-10] && (dist[X][Y]+10*sqrt(2) < dist[X-10][Y-10])) 
		{
			//cout<<"TRUE1"<<endl;
		    dist[X-10][Y-10] = dist[X][Y]+10*sqrt(2);
		    parent[X-10][Y-10].x=X;
		    parent[X-10][Y-10].y=Y;
		}
		//left
		if (isvalid(X,Y-10) && !sptSet[X][Y-10] && (dist[X][Y]+10 < dist[X][Y-10]))
		{ 
			//cout<<"TRUE1"<<endl;
		    dist[X][Y-10] = dist[X][Y]+10;
		    parent[X][Y-10].x=X;
		    parent[X][Y-10].y=Y;
		}
		//bottom-left
		if (isvalid(X+10,Y-10) && !sptSet[X+10][Y-10] && (dist[X][Y]+10*sqrt(2) < dist[X+10][Y-10])) 
		{
			//cout<<"TRUE1"<<endl;
		    dist[X+10][Y-10] = dist[X][Y]+10*sqrt(2);
		    parent[X+10][Y-10].x=X;
		    parent[X+10][Y-10].y=Y;
		}
		//bottom
		if (isvalid(X+10,Y) && !sptSet[X+10][Y] && (dist[X][Y]+10 < dist[X+10][Y]))
		{ 
			//cout<<"TRUE1"<<endl;
		    dist[X+10][Y] = dist[X][Y]+10;
		    parent[X+10][Y].x=X;
		    parent[X+10][Y].y=Y;
		}
		//bottom-right
		if (isvalid(X+10,Y+10) && !sptSet[X+10][Y+10] && (dist[X][Y]+10*sqrt(2) < dist[X+10][Y+10]))
		{ 
			//cout<<"TRUE1"<<endl;
		    dist[X+10][Y+10] = dist[X][Y]+10*sqrt(2);
		    parent[X+10][Y+10].x=X;
		    parent[X+10][Y+10].y=Y;
		}
		//right
		if (isvalid(X,Y+10) && !sptSet[X][Y+10] && (dist[X][Y]+10 < dist[X][Y+10]))
		{ 
			//cout<<"TRUE1"<<endl;
		    dist[X][Y+10] = dist[X][Y]+10;
		    parent[X][Y+10].x=X;
		    parent[X][Y+10].y=Y;
		}
		//top-right
		if (isvalid(X-10,Y+10) && !sptSet[X-10][Y+10] && (dist[X][Y]+10*sqrt(2) < dist[X-10][Y+10]))
		{ 
			//cout<<"TRUE1"<<endl;
		    dist[X-10][Y+10] = dist[X][Y]+10*sqrt(2);
		    parent[X-10][Y+10].x=X;
		    parent[X-10][Y+10].y=Y;
		}
     } 
   
     // print the constructed distance array 
     //printSolution(dist, V); 
} 

void print_path(graph_node parent[ROWS][COLS], graph_node start, graph_node end)
{
	//cout<<"k";
	if((start.x == end.x) && (start.y == end.y))
	{
		//cout<<"INTHIS";
		//img.at<uchar>(end.x,end.y)=160;
		printf("( %d , %d ) ",start.x,start.y);
	}
	else if(parent[end.x][end.y].x==-1 && parent[end.x][end.y].y==-1)
	{
		//cout<<"INTHAT";
		printf("\nNo path from (%d,%d) to (%d,%d) exists",start.x,start.y,end.x,end.y);
	}
	else
	{
		//cout<<"INWHAT";
		print_path(parent,start,parent[end.x][end.y]);
		printf("( %d , %d ) ",end.x,end.y);
		//img.at<uchar>(end.x,end.y)=160;
		//line(img,Point(end.y,end.x),Point(parent[end.x][end.y].y,parent[end.x][end.y].x),(255),2);
	}
}
int main()
{	
	//cout<<img.at<uchar>(100,100);
	graph_node start,end;
	graph_node parent[ROWS][COLS];
	start.x = 0;
	start.y = 0;
	end.x = ROWS/2;
	end.y = 0;
	//namedWindow("dijkstra_path",WINDOW_NORMAL);
	// int x_cuts = 10;
	// int y_cuts = 15;
	int number = 1;
	while(number<DATASET)
	{
		
		stringstream ss;
        ss<<(number);
        //ss<<2;
        string s = "Sample_images/";
        string s1 = "img";
        string s2 = ss.str();
        string s3 = ".jpg";
        //cout << s+s1+s2+s3 << endl;
        graph_node parent[ROWS][COLS];
        start.x = 0;
		start.y = 0;
		end.x = ROWS/2;
		end.y = 0;
        img = imread(s+s1+s2+s3,0);
        //cout << img.at<uchar>(100,100) << endl;
		for(int i=0;i<4;i++)
		{
			start.y=0;
			for(int j=0;j<3;j++)
			{
				dijkstra(start,end,parent);
				print_path(parent,start,end);
				cout<<endl;
				//imshow("dijkstra_path",img);
				//waitKey(100);
				start.y+=100;
			} 
			start.x+=50;
		}
		number++;
	}
	
}