#include<bits/stdc++.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <ctime>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

#define SSL_FIELD_WIDTH 6000
#define SSL_FIELD_HEIGHT 4500
#define ROWS 200
#define COLS 300
int DBOX_RADIUS = (int)(1000.0*ROWS/SSL_FIELD_HEIGHT);	//66
int DBOX_SIDE = (int)(500.0*ROWS/SSL_FIELD_HEIGHT);	//33
#define BOT_RADIUS 15

#define DATASET 100000
#define DBOX_BOTS 3
#define HALFLINE_BOTS 2
float PI = 180.0/3.14159265;

#define ERROR 2.2

int left_dbox_x(int i, int print)
{	
	if(i <= ROWS/2 - DBOX_SIDE/2) //i < 200 - 16 = 184
	{
		int y = ROWS/2 - i - DBOX_SIDE/2;	//y = 184 - i
		if(y<2) return DBOX_RADIUS;		//boundary case
		if(y-1 >= DBOX_RADIUS) return BOT_RADIUS;
		if(print)
		{
			printf("%d %d\n",DBOX_RADIUS,DBOX_SIDE);
			printf("i = %d\ny = %d\nacos = %lf\ntan = %lf\nx = %d\n", i, y, acos((float)(y-1)/DBOX_RADIUS)*PI, tan(acos((float)(y-1)/DBOX_RADIUS)), (int)(tan(acos((float)(y-1)/DBOX_RADIUS))*y));
		}
		int x = (int)(y*tan(acos((float)(y-1)/DBOX_RADIUS)));
		if(x >= DBOX_RADIUS) return DBOX_RADIUS;
		return x;	
	}
	
	else if(i < ROWS/2 + DBOX_SIDE/2)	//i < 200 + 16 = 216
	{
		if(print) printf("inmid\n");
		return DBOX_RADIUS;
	}

	else
	{
		int y = i - ROWS/2 - DBOX_SIDE/2;	//i - 200 - 16 = i - 216
		if(y<=2) return DBOX_RADIUS;
		if(y-1 >= DBOX_RADIUS) return BOT_RADIUS;
		if(print) printf("i = %d\ny = %d\nacos = %lf\ntan = %lf\nx = %d\n", i, y, acos((float)(y-1)/DBOX_RADIUS)*PI, tan(acos((float)(y-1)/DBOX_RADIUS)), (int)(tan(acos((float)(y-1)/DBOX_RADIUS))*y));

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

Mat draw_field_lines(Mat img)
{
	for(int i=0; i<ROWS; i++)
    {
		if(i > ROWS/2 - DBOX_SIDE/2 - DBOX_RADIUS && i < ROWS/2 + DBOX_SIDE/2 + DBOX_RADIUS)
		{
			int j1;
			//int j2;
			j1 = left_dbox_x(i,0) - 1;
			//j2 = right_dbox_x(i) - 1;
			//printf("(%d,%d)---------------\n",j1,j2);
			//if(j1<0 || j2<0) {printf("(%d,%d)\n%d\n",j1,j2,(int)(acos(23.0/27)*PI)); print=1;}
			//else print=0;
			img.at<uchar>(i,j1) = 150;
			//img.at<uchar>(i,j2) = 150;
		}
		
		img.at<uchar>(i,COLS/2) = 150;
    }

    return img;
}

Mat random_dbox_bots(Mat img, ofstream *myfile)
{
	int dbox_botPos_i[DBOX_BOTS];
	dbox_botPos_i[0] = ROWS/2 - (int)((float)DBOX_SIDE*ERROR/2) - rand()%DBOX_RADIUS + BOT_RADIUS;
	dbox_botPos_i[1] = ROWS/2 - (int)(DBOX_SIDE*ERROR/2) + rand()%((int)(DBOX_SIDE*ERROR - 2*BOT_RADIUS)) + BOT_RADIUS;
	dbox_botPos_i[2] = ROWS/2 + (int)(DBOX_SIDE*ERROR/2) + rand()%DBOX_RADIUS - BOT_RADIUS;
		
	for(int i=0; i<DBOX_BOTS; i++)
	{
		int y = dbox_botPos_i[i];
		int x = left_dbox_x(y,0) - 1;
		img = circle(img,y,x,BOT_RADIUS+1);
		*myfile << x << " " << y << " " ;
	}

	return img;
}

/*Mat defensive_dbox_bots(Mat img, ofstream *myfile)
{
	int i = ROWS/2 - (int)(DBOX_SIDE*ERROR/2) + rand()%((int)(DBOX_SIDE*ERROR - 2*BOT_RADIUS)) + BOT_RADIUS;
	int i1 = i - BOT_RADIUS;
	int i2 = i + BOT_RADIUS;
	img = circle(img,i1,DBOX_RADIUS,BOT_RADIUS+1);
	img = circle(img,i2,DBOX_RADIUS,BOT_RADIUS+1);

	*myfile << DBOX_RADIUS << " " << i1 << " " << DBOX_RADIUS << " " << i2 << " ";

	i = rand()%(DBOX_RADIUS*2) - DBOX_RADIUS;
	if(i < 0) {i = ROWS/2 - (int)(DBOX_SIDE*ERROR/2) + i + BOT_RADIUS; printf("i<0\n");}
	else {i = ROWS/2 + (int)(DBOX_SIDE*ERROR/2) + i - BOT_RADIUS; printf("i>0\n");}
	img = circle(img,i,left_dbox_x(i)-1,BOT_RADIUS+1);

	*myfile << left_dbox_x(i)-1 << " " << i << " ";

	return img;
}*/

Mat defensive_dbox_bots(Mat img, ofstream *myfile)
{
	int i = rand()%(DBOX_SIDE + 2*DBOX_RADIUS - 2*BOT_RADIUS);
	//printf("i=%d\n",i);
	int i1 = ROWS/2 - DBOX_SIDE/2 - DBOX_RADIUS + i; //118 + i
	int i2 = ROWS/2 - DBOX_SIDE/2 - DBOX_RADIUS + i + 2*BOT_RADIUS;
	int j1 = left_dbox_x(i1,0)-1;
	int j2 = left_dbox_x(i2,0)-1;
	img = circle(img,i1,j1,BOT_RADIUS+1);
	img = circle(img,i2,j2,BOT_RADIUS+1);

	*myfile << j1 << " " << i1 << " " << j2 << " " << i2 << " ";

	int i3;
	if(i <= BOT_RADIUS) 
		i3 = ROWS/2 - DBOX_SIDE/2 - DBOX_RADIUS + BOT_RADIUS + i + rand()%(DBOX_SIDE + 2*DBOX_RADIUS - 2*BOT_RADIUS - i);
	else if(i <= DBOX_SIDE + 2*DBOX_RADIUS - 3*BOT_RADIUS) //i <= 33 + 2*66 - 3*15 = 33 + 132 - 45 = 120
	{
		i3 = rand()%(DBOX_SIDE + 2*DBOX_RADIUS - 8*BOT_RADIUS) - (DBOX_SIDE + 2*DBOX_RADIUS - 8*BOT_RADIUS)/2;
		if(i3<0)
			i3 = ROWS/2 - DBOX_SIDE/2 - DBOX_RADIUS - 2*BOT_RADIUS + i + i3;
		else
			i3 = ROWS/2 - DBOX_SIDE/2 - DBOX_RADIUS + 4*BOT_RADIUS + i + i3;
	}

	else
		i3 = ROWS/2 - DBOX_SIDE/2 - DBOX_RADIUS + BOT_RADIUS + rand()%(i - 3*BOT_RADIUS);

	//printf("i3=%d\n",i3);
	int j3 = left_dbox_x(i3,0) - 1;
	img = circle(img,i3,j3,BOT_RADIUS+1);

	*myfile << j3 << " " << i3 << " ";

	return img;
}

Mat random_halfline_bots(Mat img, ofstream *myfile)
{
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
		}while(flag == 0);    		

		img = circle(img,halfline_botPos_i[i],COLS/2,BOT_RADIUS+1);

		*myfile << halfline_botPos_i[i] << " ";
	}

	*myfile << endl;

	return img;
}

int main()
{
	ofstream myfile;
  	myfile.open ("data/ssl_field_features.txt");
  
	for(int count=0; count<DATASET; count++)
	{
		stringstream ss;
        ss<<(count);
        string img_name = "defensive_dataset/img" + ss.str() + ".jpg";
        
        Mat img(ROWS,COLS,CV_8UC1,Scalar(0));

        //img = draw_field_lines(img);
    	//img = random_dbox_bots(img,myfile);
    	img = defensive_dbox_bots(img,&myfile);
    	img = random_halfline_bots(img,&myfile);
    	/*namedWindow("SHOW",WINDOW_NORMAL);
    	imshow("SHOW",img);
    	while(waitKey(10)!=27){}*/
    	imwrite(img_name,img);
    }
	return 0;
}