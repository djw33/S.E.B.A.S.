/* 
 * main.cpp tests the functionality of the Computer Vision System
 */
 
#include "ComputerVision.h"
#include <iostream>

using namespace std;

//VideoCapture * video;

int Hue1 = 19;
int Saturation1 = 185;
int Value1 = 205;
int Hue2 = 101;
int Saturation2 = 22;
int Value2 = 166;
/*
void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
	// Left click to indicate front of the vehicle
	if (event == EVENT_LBUTTONDOWN) {
		Mat imgTemp;
		bool bSuccess = video->read(imgTemp); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
		}

		cvtColor(imgTemp, imgTemp, COLOR_BGR2HSV);

		Vec3b hsv = imgTemp.at<Vec3b>(y, x);
		cout << (int)hsv.val[0] << " " << (int)hsv.val[1] << " " << (int)hsv.val[2] << endl;
		Hue1 = hsv.val[0];
		Saturation1 = hsv.val[1];
		Value1 = hsv.val[2];
	}
	// Right click to indicate back of the vehicle
	if (event == EVENT_RBUTTONDOWN) {
		Mat imgTemp;
		bool bSuccess = video->read(imgTemp); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
		}

		cvtColor(imgTemp, imgTemp, COLOR_BGR2HSV);

		Vec3b hsv = imgTemp.at<Vec3b>(y, x);
		cout << (int)hsv.val[0] << " " << (int)hsv.val[1] << " " << (int)hsv.val[2] << endl;
		Hue2 = hsv.val[0];
		Saturation2 = hsv.val[1];
		Value2 = hsv.val[2];
	}
}
*/
int main() {
	ComputerVision compv;
	float position[2];
	float velocity[2];
	float heading;
	//video = &compv.cap;
	while(true) {
		compv.update(Hue1, Saturation1, Value1, Hue2, Saturation2, Value2, position, velocity, &heading);
		cout << "Position: " << position[0] << ", " << position[1] << " | Velocity: " << velocity[0] << ", " << velocity[1] << " | Heading: " << heading << endl;
		//setMouseCallback("Original", CallBackFunc, NULL);
	}
	return 0;
}