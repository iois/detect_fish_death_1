#include <iostream>
#include "opencv2/opencv.hpp"
#include"DetectFishDeth.h"

using namespace std;
using namespace cv;

const Scalar COLOR = Scalar(255, 255, 0);

int main()
{
	Mat src_gray;
	Mat dst;

	Mat background;
	background = imread("background.bmp", 0);//0:gray

	VideoCapture vidCapture("20140109.avi");//Video179.wmv
	Mat frame;

	namedWindow("Contours", CV_WINDOW_AUTOSIZE);

	DetectFishDeth *detect_fish_deth = nullptr;

	for (;;){
		vidCapture >> frame;

		cvtColor(frame, src_gray, COLOR_RGB2GRAY);

		absdiff(src_gray, background, dst);//差分

		bitwise_xor(Scalar(255, 0, 0, 0), dst, dst);//xor,颜色取反

		GaussianBlur(dst, dst, Size(5, 5), 0, 0);

		threshold(dst, dst, 210, 255, 0);//阈值分割

		imshow("camera 1 frame", dst);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		findContours(dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<vector<Point> > another_contours;

		for (int i = 0; i < contours.size()-1; ++i){
			another_contours.push_back(contours[i + 1]);
		}

		if (!detect_fish_deth){
			detect_fish_deth = new DetectFishDeth(another_contours, 640, 900);
		}
		else{
			vector<double> prob = detect_fish_deth->get_prob_death(another_contours);

			vector<double> speed = detect_fish_deth->get_speed();
			vector<double> area = detect_fish_deth->get_diff_area();
			vector<double> poz = detect_fish_deth->get_relativepoz();
			/*
			for each (double var in speed)
			{
				cout << "speed= " << var << " ";
			}cout << endl;
			
			for each (double var in area)
			{
				cout << "area= " << var << " ";
			}cout << endl;
			
			for each (double var in poz)
			{
				cout << "poz= " << var << " ";
			}
			cout << endl;
			*/
			for each (double var in prob)
			{
				cout << "prob= " << var << " ";
			}cout << endl;
		}

		/// Draw contours
		Mat drawing = Mat::zeros(dst.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++)
		{
			//if (contours[i].size() >4)
			{
				drawContours(drawing, contours, i, COLOR, 2, 8, hierarchy, 0, Point());
				/*
				for (int j = 0; j < contours[i].size(); ++j){
				cout << contours[i][j].x << " " << contours[i][j].y << endl;
				}
				*/
			}
		}
		imshow("Contours", drawing);

		int key = waitKey(200);
		if (key == 27){ break; }
	}
	return 0;
}
