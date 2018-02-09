/* 
 * PID.cpp defines the functions for the PID class
 */

#include "PID.h"

/* PID constructor
 * parameters:  p, a float with the desired pFactor
 *				i, a float with the desired iFactor
 *				d, a float with the desired dFactor
 * result: the members of the PID class are initialized
 */
PID::PID(float p, float i, float d) {
	pFactor = p;
	iFactor = i;
	dFactor = d;
	integral[0] = integral[1] = 0;
	lastError[0] = lastError[1] = 0;
}

/* update: runs one iteration of the PID controller
 * parameters:  setpoint, an array with the desired system response
 *				actual, an array with the actual system resonse
 *				timeFrame, a float with the time since the last update
 *				result, an array to store the output of the PID controller
 * result: result is updated with the output of the PID controller
 */
void PID::update(float setpoint[], float actual[], float timeFrame, float result[]) {
	float present[2];
	arrSub(setpoint, actual, 2, present);
	
	float timeScale[2];
	arrScale(present, timeFrame, 2, timeScale);
	arrAdd(integral, timeScale, 2, integral);
	
	float difference[2];
	arrSub(present, lastError, 2, difference);
	
	float deriv[2];
	arrScale(difference, 1 / timeFrame, 2, deriv);
	lastError[0] = present[0];
	lastError[1] = present[1];
	
	float pArr[2];
	float iArr[2];
	float dArr[2];
	float sum1[2];
	arrScale(present, pFactor, 2, pArr);
	arrScale(integral, iFactor, 2, iArr);
	arrScale(deriv, dFactor, 2, dArr);
	arrAdd(pArr, iArr, 2, sum1);
	arrAdd(sum1, dArr, 2, result);
}

/* arrAdd: adds two arrays together
 * parameters:  a1, the first array to be added
 *				a2, the second array to be added
 *				size, the size of the arrays
 *				result, an array to store the sum
 * result: result stores the array of a1 + a2
 */
void arrAdd(float a1[], float a2[], int size, float result[]) {
	int i;
	for(i = 0; i < size; i++) {
		result[i] = a1[i] + a2[i];
	}
}

/* arrSub: subtracts two arrays
 * parameters:  a1, the first array
 *				a2, the second array to be subtracted from a1
 *				size, the size of the arrays
 *				result, an array to store the difference
 * result: result stores the array of a1 - a2
 */
void arrSub(float a1[], float a2[], int size, float result[]) {
	int i;
	for(i = 0; i < size; i++) {
		result[i] = a1[i] - a2[i];
	}
}

/* arrScale: scales an array by a constant
 * parameters:  a1, the array to be scaled
 *				scale, the scale factor
 *				size, the size of the arrays
 *				result, an array to store the result
 * result: result stores the array of scale * a1
 */
void arrScale(float a1[], float scale, int size, float result[]) {
	int i;
	for(i = 0; i < size; i++) {
		result[i] = a1[i] * scale;
	}
}

/* arrMagnitude: returns the magnitude of the vector represented by an array
 * parameters:  a1, the array storing the vector
 *				size, the size of the arrays
 * result: returns the magnitude of a1
 */
float arrMagnitude(float a1[], int size) {
	int i;
	float sum = 0;
	for(i = 0; i < size; i++) {
		sum += a1[i] * a1[i];
	}
	return sqrt(sum);
}