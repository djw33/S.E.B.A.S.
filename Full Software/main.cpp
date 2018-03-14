/* 
 * main.cpp tests the functionality of the Computer Vision System
 */
 
#include "client.h"
#include "controlSystem.h"
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
bool frontSet = false;
bool backSet = false;

VideoCapture capture(0); //capture the video from web cam

vector<vector<Point> > contours1init;
vector<Vec4i> hierarchy1init;
Point2f center1init;
float radius1init;
const Scalar& color1init = (0, 255, 0);

vector<vector<Point> > contours2init;
vector<Vec4i> hierarchy2init;
Point2f center2init;
float radius2init;
const Scalar& color2init = (255, 0, 0);

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
	// Left click to indicate front of the vehicle
	if (event == EVENT_LBUTTONDOWN) {
		Mat imgTemp;
		bool bSuccess = capture.read(imgTemp); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
		}

		cvtColor(imgTemp, imgTemp, COLOR_BGR2HSV);

		Vec3b hsv = imgTemp.at<Vec3b>(y, x);
		Hue1 = hsv.val[0];
		Saturation1 = hsv.val[1];
		Value1 = hsv.val[2];
		frontSet = true;
	}

	// Right click to indicate back of the vehicle
	if (event == EVENT_RBUTTONDOWN) {
		Mat imgTemp;
		bool bSuccess = capture.read(imgTemp); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
		}

		cvtColor(imgTemp, imgTemp, COLOR_BGR2HSV);
		Vec3b hsv = imgTemp.at<Vec3b>(y, x);
		Hue2 = hsv.val[0];
		Saturation2 = hsv.val[1];
		Value2 = hsv.val[2];
		backSet = true;
	}
}

int init() {
	if (!capture.isOpened()) {
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	int iLowH1 = 0;
	int iHighH1 = 179;
	int Herror1 = 8;

	int iLowS1 = 0;
	int iHighS1 = 255;
	int Serror1 = 50;

	int iLowV1 = 0;
	int iHighV1 = 255;
	int Verror1 = 50;

	int iLowH2 = 0;
	int iHighH2 = 179;
	int Herror2 = 8;

	int iLowS2 = 0;
	int iHighS2 = 255;
	int Serror2 = 50;

	int iLowV2 = 0;
	int iHighV2 = 255;
	int Verror2 = 50;

	while (!frontSet && !backSet) {
		Mat imgOriginal;
		Mat imgHSV;

		bool bSuccess = capture.read(imgOriginal); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		//Convert the captured frame from BGR to HSV
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

		Mat imgThresholded1;
		Mat imgThresholded2;

		iLowH1 = Hue1 - Herror1;
		iHighH1 = Hue1 + Herror1;

		iLowS1 = Saturation1 - Serror1;
		iHighS1 = Saturation1 + Serror1;

		iLowV1 = Value1 - Verror1;
		iHighV1 = Value1 + Verror1;

		iLowH2 = Hue2 - Herror2;
		iHighH2 = Hue2 + Herror2;

		iLowS2 = Saturation2 - Serror2;
		iHighS2 = Saturation2 + Serror2;

		iLowV2 = Value2 - Verror2;
		iHighV2 = Value2 + Verror2;

		// Threshold the image
		inRange(imgHSV, Scalar(iLowH1, iLowS1, iLowV1), Scalar(iHighH1, iHighS1, iHighV1), imgThresholded1);
		inRange(imgHSV, Scalar(iLowH2, iLowS2, iLowV2), Scalar(iHighH2, iHighS2, iHighV2), imgThresholded2);

		// Dilate the image
		dilate(imgThresholded1, imgThresholded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		// Identify the edges of a detected object
		findContours(imgThresholded1, contours1init, hierarchy1init, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		findContours(imgThresholded2, contours2init, hierarchy2init, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		if (contours1init.size() != 0)
		{
			for (int i = 0; i < contours1init.size(); i++) {
				if (contourArea(contours1init[0]) < contourArea(contours1init[i])) {
					contours1init[0] = contours1init[i];
				}
			}
			// Find the radius and position of a circle that encapsulates the edges of the object
			minEnclosingCircle(contours1init[0], center1init, radius1init);
			circle(imgOriginal, center1init, radius1init, color1init);
		}

		if (contours2init.size() != 0)
		{
			for (int i = 0; i < contours2init.size(); i++) {
				if (contourArea(contours2init[0]) < contourArea(contours2init[i])) {
					contours2init[0] = contours2init[i];
				}
			}
			// Find the radius and position of a circle that encapsulates the edges of the object
			minEnclosingCircle(contours2init[0], center2init, radius2init);
			circle(imgOriginal, center2init, radius2init, color2init);
		}

		// Draw an arrowed line between the two contours
		if (contours2init.size() != 0 & contours1init.size() != 0) {
			arrowedLine(imgOriginal, center2init, center1init, color1init);
		}

		setMouseCallback("Original", CallBackFunc, NULL);
		imshow("Thresholded Image1", imgThresholded1); //show the thresholded image
		imshow("Thresholded Image2", imgThresholded2);
		imshow("Original", imgOriginal); //show the original image
										 // Break the loop if the escape key is pressed
		if (waitKey(30) == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;
}
*/
int main() {
	//init();
	ComputerVision compv;
	Client client1;
	ControlSystem cs(0, 0);
	float position[2];
	float velocity[2];
	float heading;
	float angle;
	float force;
	float deltaTime;
	//video = &compv.cap;
	
	//setMouseCallback("Original", CallBackFunc, NULL);
	
	while(true) {
		deltaTime = compv.update(Hue1, Saturation1, Value1, Hue2, Saturation2, Value2, position, velocity, &heading);
		cs.update(position, velocity, heading, deltaTime / 1000, &angle, &force);
		client1.transmit((int)angle, (int)force);
	}
	return 0;
}