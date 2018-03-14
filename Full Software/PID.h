/* 
 * PID.h declares the functions for the PID class
 */

#ifndef PID_H_
#define PID_H_

#include <math.h>

class PID {
public:
    PID(float p, float i, float d);
	void update(float setpoint[], float actual[], float timeFrame, float result[]);
private:
    float integral[2];
    float lastError[2];
    float pFactor, iFactor, dFactor;
};

void arrAdd(float a1[], float a2[], int size, float result[]);
void arrSub(float a1[], float a2[], int size, float result[]);
void arrScale(float a1[], float scale, int size, float result[]);
float arrMagnitude(float a1[], int size);

#endif /* PID_H_ */