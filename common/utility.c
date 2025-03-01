#include "utility.h"
#include <math.h>

// Function to return the maximum of two float values
float max(float a, float b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Function to return the minimum of two float values
float min(float a, float b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

float magnitude(float a, float b, float c) {
	return sqrt(a*a + b*b + c*c);
}

int sign(float a) {
	if(a == 0.0f) {
		return 0;
	}
	return (a > 0.0f) ? 1 : -1
}
