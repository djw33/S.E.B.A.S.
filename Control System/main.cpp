/* 
 * main.cpp tests the functionality of the Control System
 */
 
#include "controlSystem.h"
#include <iostream>

using namespace std;

int main() {
	ControlSystem cs(0, 0);
	float angle;
	float force;
	float position[2] = {1, 4};
	float velocity[2] = {-1, -1};
	cs.update(position, velocity, 255, 0.1, &angle, &force);
	cout << angle << endl << force << endl;
	return 0;
}