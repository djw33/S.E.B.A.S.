/* 
 * controlSystem.cpp defines the functions for the ControlSystem class
 */

#include "controlSystem.h"
#include <iostream>
#include <opencv2/opencv.hpp>

/* ControlSystem constructor
 * parameters:  targetx, a float with the desired x coordinate of the target
 *				targety, a float with the desired y coordinate of the target
 * result: the members of the ControlSystem class are initialized
 */
ControlSystem::ControlSystem(float targetx, float targety) :
			posController(pP, pI, pD),
			angController(aP, aI, aD),
			posVelController(vP, vI, vD){
	cvCreateTrackbar("Pstin P", "PID CONTROL SYSTEM", &pP, 1000);
	cvCreateTrackbar("Pstin I", "PID CONTROL SYSTEM", &pI, 1000);
	cvCreateTrackbar("Pstin D", "PID CONTROL SYSTEM", &pD, 1000);
	cvCreateTrackbar("Angl P", "PID CONTROL SYSTEM", &aP, 1000);
	cvCreateTrackbar("Angl I", "PID CONTROL SYSTEM", &aI, 1000);
	cvCreateTrackbar("Angl D", "PID CONTROL SYSTEM", &aD, 1000);
	cvCreateTrackbar("Vel P", "PID CONTROL SYSTEM", &vP, 1000);
	cvCreateTrackbar("Vel I", "PID CONTROL SYSTEM", &vI, 1000);
	cvCreateTrackbar("Vel D", "PID CONTROL SYSTEM", &vD, 1000);
	target[0] = targetx;
	target[1] = targety;
	powerOn = 1;
}

void ControlSystem::setTarget(float targetx, float targety) {
	target[0] = targetx;
	target[1] = targety;
}

/* update: runs the control system and computes the desired force magnitude and direction
 * parameters:  position, an array with the boat's current location
 *				velocity, an array with the boat's current velocity
 *				heading, a float with the angle of the boat's heading (0 is east)
 *				deltaTime, a float the time from the last update
 *				responseAngle, a pointer to the float storing the response angle
 *				responseForce, a pointer to the float storing the response magnitude
 * result: the data in responseAngle and responseForce are updated to the desired force
 */
void ControlSystem::update(float position[], float velocity[], float heading, float deltaTime, float * responseAngle, float * responseForce) {
	// compute heading error
	float headingError = heading - atan2 (target[1] - position[1], target[0] - position[0]) * 180 / PI;
	if (headingError < 0)
		headingError += 360;
	if (headingError > 180)
		headingError -= 360;
		
	// get control system response
	float zero[2] = {0, 0};
	float headingErrorArr[2] = {headingError, 0};
	posController.update (target, position, deltaTime, posResponse);
	angController.update (zero, headingErrorArr, deltaTime, angResponse);
	posVelController.update (zero, velocity, deltaTime, posVelResponse);
	
	// adjust position response based on heading response
	float headingAdjust[2];
	if (angResponse[0] > 0) {
		headingAdjust[0] = cos(PI / 180 * (heading + 90)) * fabs(angResponse[0]) / headingScaling;
		headingAdjust[1] = sin(PI / 180 * (heading + 90)) * fabs(angResponse[0]) / headingScaling;
	} else {
		headingAdjust[0] = cos(PI / 180 * (heading - 90)) * fabs(angResponse[0]) / headingScaling;
		headingAdjust[1] = sin(PI / 180 * (heading - 90)) * fabs(angResponse[0]) / headingScaling;
	}
	float sum1[2];
	arrAdd(posResponse, posVelResponse, 2, sum1);
	arrAdd(sum1, headingAdjust, 2, response);
	
	// compute force angle and magnitude
	// ---- currently no way to power off
	float mag = arrMagnitude(response, 2) * powerOn;
	while(heading > 360) { heading -= 360; }
	if (heading < 0)
		heading += 360;
	float angForce = atan2 (response[1], response[0]) * 180 / PI;
	if (angForce < 0)
		angForce += 360;
	angForce = 2 * heading - angForce;
	while(angForce > 360) { angForce -= 360; }
	if (angForce < 0)
		angForce += 360;

	float left = angForce - heading;
	float right = heading - angForce;
	if (left < 0)
		left += 360;
	if (right < 0)
		right += 360;
	if (left > maxAng && left <= 180)// && left < reverseAng)
		angForce = heading + maxAng;
	if (right > maxAng && right < 180)// && right < reverseAng)
		angForce = heading - maxAng;

	// return desired applied force magnitude and direction	
	*responseAngle = angForce - heading;
	*responseForce = mag;
}
