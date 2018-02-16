/**
@file ComputerVision.cpp
*/

#include "ComputerVision.h"

// globals
const Scalar& color1 = (0, 255, 0);
const Scalar& color2 = (255, 0, 0);
// **** SET DATA SAMPLING RATE ****
double samplingRate = 3;
double samplingInterval = (double)1 / samplingRate;

ComputerVision::ComputerVision() : cap(0) {
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
	}
	
	lastTime = clock();
	oldVelocity = (double)0;

	oldCenter1 = Point2f(0, 0);
	oldCenter2 = Point2f(0, 0);

	Herror1 = 8;
	Serror1 = 50;
	Verror1 = 50;
	Herror2 = 8;
	Serror2 = 50;
	Verror2 = 50;

	//Create trackbars in "Control" window
	cvCreateTrackbar("Hue1", "Control", &Herror1, 179); //Hue (0 - 179)
	cvCreateTrackbar("Saturation1", "Control", &Serror1, 255); //Saturation (0 - 255)
	cvCreateTrackbar("Value1", "Control", &Verror1, 255); //Value (0 - 255)
	cvCreateTrackbar("Hue2", "Control", &Herror2, 179); //Hue (0 - 179)
	cvCreateTrackbar("Saturation2", "Control", &Serror2, 255); //Saturation (0 - 255)
	cvCreateTrackbar("Value2", "Control", &Verror2, 255); //Value (0 - 255)
}

void ComputerVision::update(int Hue1, int Saturation1, int Value1, int Hue2, int Saturation2, int Value2) {
	Mat imgOriginal;
	Mat imgHSV;
		
	// previously globals
	vector<vector<Point> > contours1;
	vector<Vec4i> hierarchy1;
	Point2f center1;
	float radius1;
		
	vector<vector<Point> > contours2;
	vector<Vec4i> hierarchy2;
	Point2f center2;
	float radius2;
		
	clock_t currentTime;
	double newVelocity;
	double acceleration;

	bool bSuccess = cap.read(imgOriginal); // read a new frame from video

	if (!bSuccess) { //if not success, break loop
		cout << "Cannot read a frame from video stream" << endl;
	}

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	Mat imgThresholded1;
	Mat imgThresholded2;

	int iLowH1 = Hue1 - Herror1;
	int iHighH1 = Hue1 + Herror1;

	int iLowS1 = Saturation1 - Serror1;
	int iHighS1 = Saturation1 + Serror1;

	int iLowV1 = Value1 - Verror1;
	int iHighV1 = Value1 + Verror1;

	int iLowH2 = Hue2 - Herror2;
	int iHighH2 = Hue2 + Herror2;

	int iLowS2 = Saturation2 - Serror2;
	int iHighS2 = Saturation2 + Serror2;

	int iLowV2 = Value2 - Verror2;
	int iHighV2 = Value2 + Verror2;

	inRange(imgHSV, Scalar(iLowH1, iLowS1, iLowV1), Scalar(iHighH1, iHighS1, iHighV1), imgThresholded1); //Threshold the image
	inRange(imgHSV, Scalar(iLowH2, iLowS2, iLowV2), Scalar(iHighH2, iHighS2, iHighV2), imgThresholded2);

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

	int heading;
	if (contours2.size() != 0 & contours1.size() != 0) {
		arrowedLine(imgOriginal, center2, center1, color1);

		// If pointed NE
		if (center1.y < center2.y && center1.x > center2.x) {
			heading = 90 - (atan((center2.y - center1.y) / (center1.x - center2.x)) * 180 / 3.1415);
		}
		// If pointed NW
		if (center1.y < center2.y && center1.x < center2.x) {
			heading = 270 + (atan((center2.y - center1.y) / -(center1.x - center2.x)) * 180 / 3.1415);
		}
		// If pointed SW
		if (center1.y > center2.y && center1.x < center2.x) {
			heading = 180 + (atan((center2.x - center1.x) / (center1.y - center2.y)) * 180 / 3.1415);
		}
		// If pointed SE
		if (center1.y > center2.y && center1.x > center2.x) {
			heading = 180 - (atan((center1.x - center2.x) / (center1.y - center2.y)) * 180 / 3.1415);
		}
	}

	// Time data is sampled
	currentTime = clock();

	// Calculate velocity and acceleration based on position and time
	if (currentTime - lastTime >= samplingInterval * CLOCKS_PER_SEC) {
		newVelocity = (sqrt(pow(center1.x - oldCenter1.x, 2) + pow(center1.y - oldCenter1.y, 2)))/ (((double)currentTime - (double)lastTime) / CLOCKS_PER_SEC);
		acceleration = (newVelocity - oldVelocity)/(((double)currentTime - (double)lastTime)/CLOCKS_PER_SEC);
		//cout << "Heading: " << heading << "   " << "Velocity :" << newVelocity << "   " << "Acceleration: " << acceleration << endl;
		oldVelocity = newVelocity;
		lastTime = currentTime;
		oldCenter1 = center1;
		oldCenter2 = center2;
	}

	imshow("Thresholded Image1", imgThresholded1); //show the thresholded image
	imshow("Thresholded Image2", imgThresholded2);
	imshow("Original", imgOriginal); //show the original image

	if (waitKey(30) == 27) { //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		cout << "esc key is pressed by user" << endl;
	}
}

