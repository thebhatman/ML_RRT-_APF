#include<bits/stdc++.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<ctime>
#include <sstream>

using namespace cv;
using namespace std;

int isValid(int i , int j , Mat img)
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
    int m,n;
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
    int m,n;
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
	int number = 10000;
	while(number--)
	{
		int obs_number = 10+ rand()%8;
		Mat img(300,300,CV_8UC1,Scalar(0));
		while(obs_number--)
		{
			int shape = rand()%3;
			if(shape == 0)
			{
				img = circle(img,rand()%img.rows,rand()%img.cols,12+rand()%10);
			}
			else if(shape == 1)
			{
			     img = square(img,rand()%img.rows,rand()%img.cols,12+rand()%10);
		  }
			else if(shape == 2)
			{
				img = triangle(img,rand()%img.rows,rand()%img.cols,12+rand()%10);
			}
		}
        //std::string s = std::to_string(number);
        stringstream ss;
        ss<<number;
        string s1 = "img";
        string s2 = ss.str();
        string s3 = ".jpg";
        imwrite(s1+s2+s3,img); 
	}
	return 0;
}