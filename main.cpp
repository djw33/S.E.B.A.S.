/* 
 * main.cpp tests the functionality of the Computer Vision System
 */
 
#include "ComputerVision.h"
#include <iostream>

using namespace std;

VideoCapture * video;

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		Mat imgTemp;
		bool bSuccess = video->read(imgTemp); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
		}

		cvtColor(imgTemp, imgTemp, COLOR_BGR2HSV);

		Vec3b hsv = imgTemp.at<Vec3b>(y, x);
		cout << (int)hsv.val[0] << " " << (int)hsv.val[1] << " " << (int)hsv.val[2] << endl;
		//Hue1 = hsv.val[0];
		//Saturation1 = hsv.val[1];
		//Value1 = hsv.val[2];
	}
	if (event == EVENT_RBUTTONDOWN) {
		Mat imgTemp;
		bool bSuccess = video->read(imgTemp); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
		}

		cvtColor(imgTemp, imgTemp, COLOR_BGR2HSV);

		Vec3b hsv = imgTemp.at<Vec3b>(y, x);
		cout << (int)hsv.val[0] << " " << (int)hsv.val[1] << " " << (int)hsv.val[2] << endl;
		//Hue2 = hsv.val[0];
		//Saturation2 = hsv.val[1];
		//Value2 = hsv.val[2];
	}
}

int main() {
	ComputerVision cv;
	video = &cv.cap;
	while(true) {
		cv.update();
		setMouseCallback("Original", CallBackFunc, NULL);
	}
	//cout << angle << endl << force << endl;
	return 0;
}

