#version 330

uniform vec2 u_pixelSize;
uniform int u_kernelSize;
uniform sampler2D u_depthTexture;
uniform sampler2D u_noiseTexture;
uniform vec3 u_kernel[40];
uniform float u_radius;
uniform float u_maxDepthDiff = 0.1;

out vec4 o_color;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	float occlusion = u_maxDepthDiff * u_kernelSize;
	vec2 pos = gl_FragCoord.xy * u_pixelSize;
	float depth = texture2D(u_depthTexture, pos).x;

	//float angle = rand(pos);
	float angle = texture2D(u_noiseTexture, gl_FragCoord.xy);

	for(int i = 0; i < u_kernelSize; ++i) {

		vec2 samplePos = u_kernel[i].xy;
		samplePos.x = u_kernel[i].x * cos(angle) - u_kernel[i].y * sin(angle);
		samplePos.y = u_kernel[i].x * sin(angle) + u_kernel[i].y * cos(angle);
		float sampleDepth = texture2D(u_depthTexture, pos + samplePos * u_pixelSize * u_radius / depth).x;

		float depthDiff = depth - sampleDepth;

		if (depthDiff > 0 && depthDiff < u_maxDepthDiff) {
			occlusion -= u_maxDepthDiff - depthDiff;
		}
	}

	o_color = vec4(vec3(occlusion), 1.0);
}
