/* 
 * controlSystem.h declares the functions for the ControlSystem class
 */

#ifndef CONTROLSYSTEM_H_
#define CONTROLSYSTEM_H_

#include "PID.h"
#include <math.h>

#define maxAng 45
#define reverseAng 180
#define headingScaling 60
#define positionP 100
#define positionI 0.002
#define positionD 120
#define angleP 60
#define angleI 0.002
#define angleD 0.1
#define PI 3.14159265

class ControlSystem {
public:
    ControlSystem(float targetx, float targety);
	void setTarget(float targetx, float targety);
    void update(float position[], float velocity[], float heading, float deltaTime, float * responseAngle, float * responseForce);
private:
	float target[2];
	PID posController;
	PID angController;
	float posResponse[2];
	float angResponse[2];
	float backx;
	float backy;
	float backz;
	int powerOn;
};

#endif /* CONTROLSYSTEM_H_ */