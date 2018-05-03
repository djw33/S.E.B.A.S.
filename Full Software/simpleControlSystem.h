/* 
 * controlSystem.h declares the functions for the ControlSystem class
 */

#ifndef SIMPLECONTROLSYSTEM_H_
#define SIMPLECONTROLSYSTEM_H_

#include "PID.h"
#include <math.h>

#define maxAng 65
#define angleScaleFactor 40*125/maxAng
#define positionP 42.5
#define positionI 0.002
#define positionD 94.5
#define angleP 60
#define angleI 0.002
#define angleD 37.2
#define PI 3.14159265
#define SMALL_CIRCLE 50
#define LARGE_CIRCLE 100

class SimpleControlSystem {
public:
	SimpleControlSystem(float targetx, float targety);
	void setTarget(float targetx, float targety);
    void update(float position[], float velocity[], float heading, float deltaTime, float * responseAngle, float * responseForce);
private:
	float target[2];
	PID posController;
	PID angController;
	int pP = int(positionP * 10);
	int pI = int(positionI * 100000);
	int pD = int(positionD * 10);
	int aP = int(angleP * 10);
	int aI = int(angleI * 100000);
	int aD = int(angleD * 10);
	float posResponse[2];
	float angResponse[2];
	float backx;
	float backy;
	float backz;
	int powerOn;
};

#endif /* SIMPLECONTROLSYSTEM_H_ */