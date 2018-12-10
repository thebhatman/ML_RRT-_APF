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
	int number = 10000;
	ofstream myfile;
  	myfile.open ("data/features.txt");
	while(number>=1)
	{
		stringstream ss;
        ss<<(10000-number);
        string s = "dataset/";
        string s1 = "img";
        string s2 = ss.str();
        string s3 = ".jpg";
        int num = 0;
        int x = 0;
        int y = 0;
        img = imread(s+s1+s2+s3,0);	
        //cout << img.rows << endl;
        for (int i = 0 ; i < img.rows ; i++)
        {
        	for (int j = 0 ; j < img.cols ; j++)
        	{
        		if (img.at<uchar>(i,j)>200)
        		{
        			num++;
        			x+=i;
        			y+=j;
        		}
        	}
        }
        number--;
        myfile << x/num << " " << y/num << " " <<num << endl;
    }
}