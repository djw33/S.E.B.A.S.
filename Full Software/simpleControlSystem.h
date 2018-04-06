/* 
 * controlSystem.h declares the functions for the ControlSystem class
 */

#ifndef SIMPLECONTROLSYSTEM_H_
#define SIMPLECONTROLSYSTEM_H_

#include "PID.h"
#include <math.h>

#define maxAng 45
#define positionP 102.7
#define positionI 0.002
#define positionD 94.5
#define angleP 60
#define angleI 0.002
#define angleD 37.2
#define PI 3.14159265

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