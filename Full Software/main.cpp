/* 
 * main.cpp tests the functionality of the Computer Vision System
 */
 
#include "client.h"
#include "controlSystem.h"
#include "simpleControlSystem.h"
#include "ComputerVision.h"
#include <iostream>

#define SYSTEM_START 32

using namespace std;

int main() {
	ComputerVision compv;
	Client client1;
	SimpleControlSystem cs(0, 0);
	float position[2];
	float velocity[2];
	float heading;
	float angle;
	float force;
	float deltaTime;
	int oldTargetX = 0;
	int oldTargetY = 0;
	int targetX = 0;
	int targetY = 0;
	bool start = false;
	bool newData = false;
	
	while(true) {
		if (compv.update(position, velocity, &heading, &targetX, &targetY, &deltaTime, &newData) == SYSTEM_START) start = true;
		
		if (start && newData) {
			cs.update(position, velocity, heading, deltaTime / 1000, &angle, &force);
			cout << (int)angle << ", " << (int)force << endl;
			client1.transmit((int)angle, (int)force);
		}

		if (targetX != oldTargetX || targetY != oldTargetY) {
			cs.setTarget(targetX, targetY);
			oldTargetX = targetX;
			oldTargetY = targetY;
		}
	}
	return 0;
}