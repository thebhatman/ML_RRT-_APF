#include<bits/stdc++.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<ctime>

using namespace cv;
using namespace std;

int isValid(int i , int j , Mat a)
{
	if ( i < img.rows && j < img.cols && i > -1 && j > -1)
		return 1;
	return 0;
}

Mat circle(Mat a, int i, int j, int r)
{
    int p,q;
    for(p=i-r;p<i+r;p++)
        for(q=j-sqrt(pow(r,2)-pow(p-i,2));q<j+sqrt(pow(r,2)-pow(p-i,2));q++)
        {
            if(isValid(q,p,a))
            a.at<uchar>(p,q)=255;
        }

    return a;
}

Mat square(Mat a , int j , int k , int r )
{
    for(m = j - r; m <= j + r; m++)
    {
        for(n = k - r; n <= k + r; n++)
        {
            if(isValid(m, n, a))
            {
                a.at<uchar>(m,n) = 255;
            }
        }
    }
    return a;
}

Mat triangle(Mat a , int j , int k , int r )
{
    for(m = j - r; m <= j + r; m++)
    {
        for(n = m; n <= k + r; n++)
        {
            if(isValid(m, n, a))
            {
                a.at<uchar>(m,n) = 255;
            }
        }
    }
    return a;
}

int main()
{
	int a[3] = {0,1,2};
	srand(time(0));
	number = 1;
	while(number--)
	{
		obs_number = 7+ rand()%8;
		Mat img(300,300,CV_8UC1,Scalar(0));
		while(obs_number--)
		{
			shape = rand()%3;
			if(shape == 0)
			{
				a = circle(a,rand()%img.rows,rand()%img.cols,10+rand()%10);
			}
			else if(shape == 1)
			{
				a = square(a,rand()%img.rows,rand()%img.cols,10+rand()%10);
			}
			else if(shape == 2)
			{
				a = triangle(a,rand()%img.rows,rand()%img.cols,10+rand()%10);
			}
		}
	}
	imwrite(strcat(strcat("img",number.str()),".jpg"),img);	
}