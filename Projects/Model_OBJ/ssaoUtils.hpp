#include <random>
#include "../ESGIGL/common/vector.h"
#include <iostream>

#define MAX_KERNEL_SIZE 128
#define PI 3.14159265

const int kernelSize = 64;
vec3 kernel[MAX_KERNEL_SIZE];
const int noiseSize = 16;
vec3 noise[noiseSize];

float random(float a, float b) {
	return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

float lerp(float a, float b, float value) {
	return value / (b - a) + a;
}

void initKernel() {
	for (int i = 0; i < MAX_KERNEL_SIZE; ++i) {
		kernel[i] = vec3(
			random(-1.0f, 1.0f),
			random(-1.0f, 1.0f),
			random(0.0f, 1.0f)
			);
		//clamp to unit hemisphere
		kernel[i].Normalize();

		//What we actually want is for the distance from the origin to falloff as we generate more points
		float scale = float(i) / float(kernelSize);
		scale = lerp(0.1, 1.1, scale * scale);
		kernel[i] = kernel[i] * scale;
	}
}

void initNoiseTexture() {
	for (int i = 0; i < noiseSize; ++i) {
		noise[i] = vec3(
			random(0.0f, 1.0f),
			random(0.0f, 1.0f),
			0.0
			);

		noise[i].Normalize();

		//std::cout << noise[i].x << ", " << noise[i].y << ", " << noise[i].z << std::endl;
	}
}