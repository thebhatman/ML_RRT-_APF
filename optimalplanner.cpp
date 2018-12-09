#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
using namespace std;
using namespace cv;
Mat img=imread("img0.jpg",0);
struct graph_node
{
	int x,y;
};
struct node
{
	graph_node data;
	node* next;
};

struct QUEUE
{
	node *front, *rear ;
};

void init(QUEUE **qP)//initializes the front and rear pointers
{
	(*qP) = (QUEUE *)malloc(sizeof(QUEUE));
	(*qP)->front=NULL;
	(*qP)->rear=NULL;
}
int isempty(QUEUE *qP)//prints 1 if the queue is emptry, else 0
{
	int k;
	if( qP->front==NULL && qP->rear==NULL)
		k=1;
	else
		k=0;
	return k;

}
void enqueue(QUEUE *qP, graph_node data)
{
	node* temp;
	temp=(node*)malloc(sizeof(node));
	temp->data=data;
	temp->next=NULL;
	if(qP->front==NULL)
		qP->front=qP->rear=temp;
	else
	{
		(qP->rear)->next=temp;
		qP->rear=temp;
	}
}
graph_node dequeue(QUEUE *qP)
{
	if (qP->front == NULL)
		printf("Queue Empty ");
	else
	{
		node* temp;
		graph_node val;
		temp = qP->front;
		val=(qP->front)->data;
		qP->front = (qP->front)->next;
		temp->next = NULL;
		if (qP->front == NULL)
			qP->rear = NULL;
		return val;
	}
}
int isvalid( int i, int j)
{
	 if(i>=0 && j>=0 && i<img.rows && j<img.cols)
		return 1;
	else
		return 0;
}	
void BFS(graph_node parent[110][110],graph_node v)
{
	int i,j,k,l,visited[110][110];
	int distance[110][110];
	graph_node current,temp;

	for(i=0;i<100;i++)
		for(j=0;j<100;j++)
		{
			parent[i][j].x=-1;
			parent[i][j].y=-1;
			visited[i][j]=0;
			distance[i][j]=0;
		}

	QUEUE *qP=NULL;
	init(&qP);
	enqueue(qP,v);
	visited[v.x][v.y]=1;
	distance[v.x][v.y]=0;
	parent[v.x][v.y].x=-1;
	parent[v.x][v.y].y=-1;
	printf("\nBFS Traversal:");
	while(!isempty(qP))
	{
		current=dequeue(qP);
		for(k=-1;k<=1;k++)
			for(l=-1;l<=1;l++)
			{
				if(!(k==0 and l==0) && isvalid(current.x+k,current.y+l) && visited[current.x+k][current.y+l]==0 && img.at<uchar>(current.x+k,current.y+l)<100)
				{
					visited[current.x+k][current.y+l]=1;
					parent[current.x+k][current.y+l]=current;
					distance[current.x+k][current.y+l]=distance[current.x][current.y]+1;
					temp.x=current.x+k;
					temp.y=current.y+l;
					enqueue(qP,temp);
				}
			}
	}
}

void print_path(graph_node parent[110][110],graph_node start,graph_node end)
{
	if((start.x == end.x) && (start.y == end.y))
	{
		img.at<uchar>(end.x,end.y)=160;
		printf(" (%d,%d) ",start.x,start.y);
	}
	else if(parent[end.x][end.y].x==-1 && parent[end.x][end.y].y==-1)
		printf("\nNo path from (%d,%d) to (%d,%d) exists",start.x,start.y,end.x,end.y);
	else
	{
		print_path(parent,start,parent[end.x][end.y]);
		printf(" (%d,%d) ",end.x,end.y);
		img.at<uchar>(end.x,end.y)=160;
	}
}

int main()
{	
	namedWindow("chaljabhai",WINDOW_NORMAL);
	graph_node start,end;
	graph_node parent[110][110];
	// printf("Input start vertex: ");
	// scanf("%d",&start.x);
	// scanf("%d",&start.y);
	// printf("Input end vertex: ");
	// scanf("%d",&end.x);
	// scanf("%d",&end.y);
	start.x = 0;
	start.y = 0;
	end.x = 99;
	end.y = 99;
	BFS(parent,start);
	print_path(parent,start,end);
	imshow("chaljabhai",img);
	waitKey(0);
	return 0;
}