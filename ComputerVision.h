#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace cv;
using namespace std;

class ComputerVision {
public:
	ComputerVision();
	void update(int Hue1, int Saturation1, int Value1, int Hue2, int Saturation2, int Value2, float * positionOut, float * velocityOut, float * headingOut);
	VideoCapture cap;

private:
	clock_t lastTime;
	float oldVehicleX;
	float oldVehicleY;
	
	int Herror1; //8
	int Serror1; //50
	int Verror1; //50
	int Herror2; //8
	int Serror2; //50
	int Verror2; //50
};