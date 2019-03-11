#include<bits/stdc++.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <ctime>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

#define SSL_FIELD_WIDTH 10400
#define SSL_FIELD_HEIGHT 7400
#define ROWS 200
#define COLS 300
int DBOX_RADIUS = (int)(1500.0*ROWS/SSL_FIELD_HEIGHT);	//40
int DBOX_SIDE = (int)(750.0*ROWS/SSL_FIELD_HEIGHT);	//20
#define BOT_RADIUS 10

#define DATASET 10
#define DBOX_BOTS 3
#define HALFLINE_BOTS 2
float PI = 180.0/3.14159265;
int print = 1;

#define ERROR 2.1

int left_dbox_x(int i)
{	
	if(i <= ROWS/2 - DBOX_SIDE/2) //i < 100 - 10 = 90
	{
		int y = ROWS/2 - i - DBOX_SIDE/2;	//y = 90 - i
		if(y<2) return DBOX_RADIUS;		//boundary case
		/*if(print)
		{
			printf("%d\n",DBOX_RADIUS);
			printf("i = %d\ny = %d\nacos = %lf\ntan = %lf\nx = %d\n", i, y, acos((float)(y-1)/DBOX_RADIUS)*PI, tan(acos((float)(y-1)/DBOX_RADIUS)), (int)(tan(acos((float)(y-1)/DBOX_RADIUS))*y));
		}*/
		int x = (int)(y*tan(acos((float)(y-1)/DBOX_RADIUS)));
		if(x >= DBOX_RADIUS) return DBOX_RADIUS;
		return x;	
	}
	
	else if(i < ROWS/2 + DBOX_SIDE/2)	//i < 100 + 10 = 110
	{
		//printf("inmid\n");
		return DBOX_RADIUS;
	}

	else
	{
		int y = i - ROWS/2 - DBOX_SIDE/2;	//i - 110
		if(y<=2) return DBOX_RADIUS;
		//printf("i = %d\ny = %d\nacos = %lf\ntan = %lf\nx = %d\n", i, y, acos((float)(y-1)/DBOX_RADIUS)*PI, tan(acos((float)(y-1)/DBOX_RADIUS)), (int)(tan(acos((float)(y-1)/DBOX_RADIUS))*y));

		int x = (int)(y*tan(acos((float)(y-1)/DBOX_RADIUS)));
		if(x >= DBOX_RADIUS) return DBOX_RADIUS;
		return x;
	}
}

int right_dbox_x(int i)
{
	if(i <= ROWS/2 - DBOX_SIDE/2)
	{
		int y = ROWS/2 - i - DBOX_SIDE/2;
		if(y<2) return DBOX_RADIUS;
		int x = COLS - (int)(y*tan(acos((float)(y-1)/DBOX_RADIUS)));
		if(x <= COLS - DBOX_RADIUS) return COLS - DBOX_RADIUS;
		return x;	
	}
	
	else if(i < ROWS/2 + DBOX_SIDE/2)
	{
		return COLS - DBOX_RADIUS;
	}

	else
	{

		int y = i - ROWS/2 - DBOX_SIDE/2;
		if(y<2) return DBOX_RADIUS;
		int x = COLS - (int)(y*tan(acos((float)(y-1)/DBOX_RADIUS)));
		if(x <= COLS - DBOX_RADIUS) return COLS - DBOX_RADIUS;
		return x;
	}
}

int isValid(int i , int j , Mat img, int r)
{
    if ( i < img.rows && j < img.cols && i >= 0 && j >= 0)
        return 1;
    return 0;
}

Mat circle(Mat a, int i, int j, int r)
{
    int p,q;
    //i = i-r;
    //j = j-r;
    for(p=i-r;p<i+r;p++)
        for(q=j-sqrt(pow(r,2)-pow(p-i,2));q<j+sqrt(pow(r,2)-pow(p-i,2));q++)
        {
            if(isValid(q,p,a,r))
            a.at<uchar>(p,q)=255;
        }

    return a;
}

int main()
{
	ofstream myfile;
  	myfile.open ("data/ssl_field_features.txt");
  
	for(int count=0; count<DATASET; count++)
	{
		stringstream ss;
        ss<<(count);
        string img_name = "img" + ss.str() + ".jpg";	//20 60 45
        
        Mat img(ROWS,COLS,CV_8UC1,Scalar(0));

        for(int i=0; i<ROWS; i++)
        {
    		if(i > ROWS/2 - DBOX_SIDE/2 - DBOX_RADIUS && i < ROWS/2 + DBOX_SIDE/2 + DBOX_RADIUS)
    		{
    			int j1, j2;
    			j1 = left_dbox_x(i) - 1;
    			j2 = right_dbox_x(i) - 1;
    			//printf("(%d,%d)---------------\n",j1,j2);
    			//if(j1<0 || j2<0) {printf("(%d,%d)\n%d\n",j1,j2,(int)(acos(23.0/27)*PI)); print=1;}
    			//else print=0;
    			img.at<uchar>(i,j1) = 150;
    			img.at<uchar>(i,j2) = 150;
    		}
    		
    		img.at<uchar>(i,COLS/2) = 150;
        }

    	int dbox_botPos_i[DBOX_BOTS];
    	dbox_botPos_i[0] = ROWS/2 - (int)((float)DBOX_SIDE*ERROR/2) - rand()%DBOX_RADIUS + BOT_RADIUS;
    	dbox_botPos_i[1] = ROWS/2 - (int)(DBOX_SIDE*ERROR/2) + rand()%((int)(DBOX_SIDE*ERROR - 2*BOT_RADIUS)) + BOT_RADIUS;
    	dbox_botPos_i[2] = ROWS/2 + (int)(DBOX_SIDE*ERROR/2) + rand()%DBOX_RADIUS - BOT_RADIUS;
    		
    	for(int i=0; i<DBOX_BOTS; i++)
    		img = circle(img,dbox_botPos_i[i],left_dbox_x(dbox_botPos_i[i])-1,BOT_RADIUS);

    	int halfline_botPos_i[HALFLINE_BOTS];
    	for(int i=0; i<HALFLINE_BOTS; i++)
    	{	    		
    		int flag;
    		do
    		{
    			flag = 1;
    			halfline_botPos_i[i] = BOT_RADIUS + rand()%(ROWS - 2*BOT_RADIUS);
    			for(int j=0; j<i; j++)
    		    	if(abs(halfline_botPos_i[j] - halfline_botPos_i[i]) < 2*BOT_RADIUS) flag=0;
    		}while(flag==0);    		

    		img = circle(img,halfline_botPos_i[i],COLS/2,BOT_RADIUS);
    	}

    	/*namedWindow("SHOW",WINDOW_NORMAL);
    	if(count < 10)
    	{
    		imshow("SHOW",img);
    		while(waitKey(10)!=27){}
    	}*/
    	imwrite(img_name,img);
    }
	return 0;
}