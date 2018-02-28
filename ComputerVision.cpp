/**
@file project1.cpp
*/

// github.com/djw33/sebas

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace cv;
using namespace std;

VideoCapture cap(0); //capture the video from web cam

int Hue1 = 0;
int Saturation1 = 0;
int Value1 = 0;

int Hue2 = 0;
int Saturation2 = 0;
int Value2 = 0;

vector<vector<Point> > contours1;
vector<Vec4i> hierarchy1;
Point2f center1;
float radius1;
const Scalar& color1 = (0, 255, 0);

vector<vector<Point> > contours2;
vector<Vec4i> hierarchy2;
Point2f center2;
float radius2;
const Scalar& color2 = (255, 0, 0);

// **** SET DISTANCE BETWEEN LEDS (CM) ****
float ledDistance = 2;
// ****************************************
float pixels_to_cm;

// ***** SET DATA SAMPLING RATE (PER SECOND) ******
double samplingRate = 3;
// ***********************************

double samplingInterval = (double)1 / samplingRate;

clock_t lastTime;
clock_t currentTime;

int heading;

float vehicleX;
float vehicleY;

float oldVehicleX;
float oldVehicleY;

float deltaX;
float deltaY;

double velocityX;
double velocityY;

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
	// Left click to indicate front of the vehicle
	if (event == EVENT_LBUTTONDOWN) {
		Mat imgTemp;
		bool bSuccess = cap.read(imgTemp); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
		}

		cvtColor(imgTemp, imgTemp, COLOR_BGR2HSV);

		Vec3b hsv = imgTemp.at<Vec3b>(y, x);
		Hue1 = hsv.val[0];
		Saturation1 = hsv.val[1];
		Value1 = hsv.val[2];
	}

	// Right click to indicate back of the vehicle
	if (event == EVENT_RBUTTONDOWN) {
		Mat imgTemp;
		bool bSuccess = cap.read(imgTemp); // read a new frame from video

		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
		}

		cvtColor(imgTemp, imgTemp, COLOR_BGR2HSV);
		Vec3b hsv = imgTemp.at<Vec3b>(y, x);
		Hue2 = hsv.val[0];
		Saturation2 = hsv.val[1];
		Value2 = hsv.val[2];
	}
}

int main(int argc, char** argv) {
	if (!cap.isOpened())  {
		cout << "Cannot open the web cam" << endl;
		return -1;
	}
	
	lastTime = clock();

	heading = 0;

	vehicleX = 0;
	vehicleY = 0;

	oldVehicleX = 0;
	oldVehicleY = 0;

	deltaX = 0;
	deltaY = 0;

	velocityX = 0;
	velocityY = 0;

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

	while (true) {
		Mat imgOriginal;
		Mat imgHSV;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

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
		findContours(imgThresholded1, contours1, hierarchy1, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		findContours(imgThresholded2, contours2, hierarchy2, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		if (contours1.size() != 0)
		{
			for (int i = 0; i < contours1.size(); i++) {
				if (contourArea(contours1[0]) < contourArea(contours1[i])) {
					contours1[0] = contours1[i];
				}
			}
			// Find the radius and position of a circle that encapsulates the edges of the object
			minEnclosingCircle(contours1[0], center1, radius1);
			circle(imgOriginal, center1, radius1, color1);
		}

		if (contours2.size() != 0)
		{
			for (int i = 0; i < contours2.size(); i++) {
				if (contourArea(contours2[0]) < contourArea(contours2[i])) {
					contours2[0] = contours2[i];
				}
			}
			// Find the radius and position of a circle that encapsulates the edges of the object
			minEnclosingCircle(contours2[0], center2, radius2);
			circle(imgOriginal, center2, radius2, color2);
		}

		// Draw an arrowed line between the two contours
		if (contours2.size() != 0 & contours1.size() != 0) {
			arrowedLine(imgOriginal, center2, center1, color1);

			// If pointed NE
			if (center1.y < center2.y && center1.x > center2.x) {
				heading = (atan((center2.y - center1.y) / (center1.x - center2.x)) * 180 / 3.1415);
			}
			// If pointed NW
			if (center1.y < center2.y && center1.x < center2.x) {
				heading = 180 - (atan((center2.y - center1.y) / (center2.x - center1.x)) * 180 / 3.1415);
			}
			// If pointed SW
			if (center1.y > center2.y && center1.x < center2.x) {
				heading = 180 + (atan((center1.y - center2.y) / (center2.x - center1.x)) * 180 / 3.1415);
			}
			// If pointed SE
			if (center1.y > center2.y && center1.x > center2.x) {
				heading = 360 - (atan((center1.y - center2.y) / (center1.x - center2.x)) * 180 / 3.1415);
			}
		}
		
		// Get cm/pixel ratio
		pixels_to_cm = ledDistance / sqrt(pow(center1.x - center2.x, 2)+pow(center1.y - center2.y, 2));

		// Sample the clock value
		currentTime = clock();

		// Calculate position and velocity vectors if the sampling period has passed
		if (currentTime - lastTime >= samplingInterval * CLOCKS_PER_SEC) {
			vehicleX = ((center1.x + center2.x) / 2) * pixels_to_cm;
			vehicleY = ((center1.y + center2.y) / 2) * pixels_to_cm;

			deltaX = vehicleX - oldVehicleX;
			deltaY = -(vehicleY - oldVehicleY);

			velocityX = (deltaX)/(((double)currentTime - (double)lastTime) / CLOCKS_PER_SEC);
			velocityY = (deltaY)/(((double)currentTime - (double)lastTime) / CLOCKS_PER_SEC);

			/*
			PUT CODE HERE TO OUTPUT POSITION, VELOCITY, AND HEADING VALUES USING COUT
			OR SEND TO RASPBERRY PI
			*/

			lastTime = currentTime;
			oldVehicleX = vehicleX;
			oldVehicleY = vehicleY;
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

