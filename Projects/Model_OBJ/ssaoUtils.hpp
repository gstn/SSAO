#include <random>
#include "../ESGIGL/common/vector.h"

#define PI 3.14159265

const int kernelSize = 10;
float kernel[3 * kernelSize];
const int noiseSize = 16;
vec3 noise[noiseSize];

float random(float a, float b) {
	return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

void initKernel() {
	for (int i = 0; i < kernelSize; ++i) {
		vec3 sample = vec3(
			random(-1.0f, 1.0f),
			random(-1.0f, 1.0f),
			random(0.0f, 1.0f)
			);
		//clamp to unit hemisphere
		sample.Normalize();

		//focus sample points towards origin
		sample = sample * random(0.0f, 1.0f);

		//What we actually want is for the distance from the origin to falloff as we generate more points
		//float scale = float(i) / float(kernelSize);
		//scale = lerp(0.1, 1.0, scale * scale);
		//kernel[i] *= scale;

		int j = 3 * i;
		kernel[j] = sample.x;
		kernel[j + 1] = sample.y;
		kernel[j + 2] = sample.z;
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
	}
}