/*背景差分*/
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


Mat src_gray;
Mat dst;
Scalar color = Scalar(255, 255, 0);
int main()
{
	Mat background;
	background = imread("background.bmp",0);//0:gray

	VideoCapture vidCapture("Video179.wmv");
	Mat frame;

	namedWindow("Contours", CV_WINDOW_AUTOSIZE);

	for (;;){
		vidCapture >> frame;

		cvtColor(frame, src_gray, COLOR_RGB2GRAY);

		absdiff(src_gray, background, dst);//差分
		
		bitwise_xor(Scalar(255, 0, 0, 0),dst, dst);//xor,颜色取反

		GaussianBlur(dst, dst, Size(5, 5), 0, 0);
		
		threshold(dst, dst, 230, 255, 0);//阈值分割

		imshow("camera 1 frame", dst);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		findContours(dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		/// Draw contours
		Mat drawing = Mat::zeros(dst.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++)
		{
			if (contours[i].size() >4){
				drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());

				for (int j = 0; j < contours[i].size(); ++j){
					cout << contours[i][j].x << " " << contours[i][j].y << endl;
				}
			}
		}
		imshow("Contours", drawing);
		
		int key = waitKey(66);
		if (key == 27){break;}
	}
	return 0;
}
