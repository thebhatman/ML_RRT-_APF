#include<bits/stdc++.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<ctime>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

Mat img;

int main()
{
	int number = 100000;
	ofstream myfile;
  	myfile.open ("data/features.txt");
  	int num,x,y;
  	string s,s1,s2,s3;
	while(number>=1)
	{
		stringstream ss;
		// if (100000-number==46242)
		// {
		// 	myfile << x/num << " " << y/num << " " <<num << endl;
		// 	continue;
		// }

        ss<<(100000-number);
        
        s = "dataset/";
        s1 = "img";
        s2 = ss.str();
        s3 = ".jpg";
        num = 0;
        x = 0;
        y = 0;
        img = imread(s+s1+s2+s3,0);	
        //cout << img.rows << endl;
        for (int i = 0 ; i < img.rows ; i++)
        {
        	for (int j = 0 ; j < img.cols ; j++)
        	{
        		//cout << (int)img.at<uchar>(i,j) << " ";
        		if ((int)img.at<uchar>(i,j)>150)
        		{
        			//cout << i << " " << j << " " << (int)img.at<uchar>(i,j) << endl;
        			num++;
        			x+=i;
        			y+=j;
        		}
        	}
        }
        number--;
        //cout << 100000-number << " " << num << endl;
        if (num==0)
        	myfile << 50 << " " << 50 << " " << 1 << endl;
        else
        	myfile << x/num << " " << y/num << " " <<num << endl;
    }
}