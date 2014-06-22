uniform vec3 pixelSize;
uniform int u_kernelSize;
uniform sampler2D u_depthTexture;
uniform vec3 u_kernel[20];
uniform sampler2D u_rotationTexture;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	float occlusion = 1.0;
	vec2 pos = gl_FragCoord.xy * pixelSize;
	vec3 depth = vec3(texture2D(u_depthTexture, pos));

	vec3 result = vec3(1.0);
	float sampleOcclusion = 1 / u_kernelSize;

	float angle = rand(pos);

	for(int i = 0; i < u_kernelSize; ++i) {

		vec2 samplePos = u_kernel[i].xy;
		samplePos.x = u_kernel[i].x * cos(angle) - u_kernel[i].y * sin(angle);
		samplePos.y = u_kernel[i].x * sin(angle) + u_kernel[i].y * cos(angle);
		vec3 sampleDepth = texture2D(u_depthTexture, pos + samplePos * pixelSize * 15);

		result = sampleDepth;

		float depthDiff = depth.x - sampleDepth.x;

		if (depthDiff > 0 && depthDiff < 0.01) {
			//occlusion -= 0.05;
			occlusion -= .01 / pow(depthDiff, 0.2);
		}
	}

	gl_FragColor = vec4(vec3(occlusion + .1), 1.0);
}
