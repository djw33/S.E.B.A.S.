/* 
 * controlSystem.cpp defines the functions for the ControlSystem class
 */

#include "controlSystem.h"
#include <iostream>

/* ControlSystem constructor
 * parameters:  targetx, a float with the desired x coordinate of the target
 *				targety, a float with the desired y coordinate of the target
 * result: the members of the ControlSystem class are initialized
 */
ControlSystem::ControlSystem(float targetx, float targety) :
			posController(positionP, positionI, positionD),
			angController(angleP, angleI, angleD) {
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
	
	float magnitude = sqrt(pow(position[0]-target[0],2) + pow(position[1]-target[1],2));
	float positionErrorArr[2] = {magnitude, 0};
	
	posController.update (positionErrorArr, zero, deltaTime, posResponse);
	angController.update (zero, headingErrorArr, deltaTime, angResponse);
	
	// adjust responses
	if (posResponse[0] < 0) posResponse[0] = 0;

	float angle = angResponse[0] * -1;
	if (angle > maxAng) angle = maxAng;
	if (angle < -1 * maxAng) angle = -1 * maxAng;

	// return desired applied force magnitude and direction	
	*responseAngle = angle;
	*responseForce = posResponse[0];
}
