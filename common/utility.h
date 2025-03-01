#include <stdio.h>
#include <stdbool.h>

typedef struct {
	float x;
	float y;
	float z;
	bool isValid;
} vec3_t;

float min(float a, float b);

float max(float a, float b);

float magnitude(float a, float b, float c);

int sign(float a);