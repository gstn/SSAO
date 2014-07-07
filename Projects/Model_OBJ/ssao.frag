#version 330

const int MAX_KERNEL_SIZE = 128;
uniform vec2 u_pixelSize;
uniform vec2 u_noiseScale;
uniform vec3 u_kernel[MAX_KERNEL_SIZE];
uniform int u_kernelSize;
uniform float u_radius;
uniform float u_ssaoPower = 3.0;
uniform mat4 u_ProjectionMatrix;
uniform sampler2D u_depthTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_noiseTexture;
uniform sampler2D u_positionTexture;

uniform mat4 u_persMatrix;
uniform float u_near;
uniform float u_far;
uniform vec4 u_frustum;

in vec2 v_uv;

out vec4 o_color;

float ssao(in mat3 kernelBasis, in mat4 projectionInv, in vec3 origin) {
	float occlusion = 0.0;

	for(int i = 0; i < u_kernelSize; ++i) {
		//get sample position
		vec3 sample = kernelBasis * u_kernel[i];
		sample = sample * u_radius + origin;

		//project sample position
		vec4 offset = vec4(sample, 1.0);
		offset = u_persMatrix * offset;
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5 + 0.5;

		//get sample depth
		float sampleDepth = texture2D(u_depthTexture, offset.xy).z;
		vec4 samplePos = projectionInv * vec4(vec3(offset.xy, sampleDepth), 1.0);
		sampleDepth = samplePos.z / samplePos.w;

		//range check and accumulate
		float rangeCheck = smoothstep(0.0, 1.0, u_radius / abs(origin.z - sampleDepth));
		occlusion += rangeCheck * (1.0 - step(sampleDepth, sample.z));
	}

	occlusion = 1.0 - (occlusion / float(u_kernelSize));
	return pow(occlusion, u_ssaoPower);
}

void main() {
	float depth = texture2D(u_depthTexture, v_uv).x;
	
	if(depth == 1.0) {
		o_color = vec4(1.0);
		return;
	}

	vec3 normal = texture2D(u_normalTexture, v_uv).xyz - 0.5;
	normal = normalize(normal);

	mat4 projectionInv = inverse(u_persMatrix);

	vec3 windowSpacePosition = vec3(v_uv * 2.0 - 1.0, depth);
	vec4 origin = projectionInv * vec4(windowSpacePosition, 1.0);
	origin /= origin.w;

	//construct change of basis matrix btn
	vec3 rvec = texture2D(u_noiseTexture, gl_FragCoord.xy / 4.0).xyz * 2.0 - 1.0;
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);

	float occlusion = ssao(tbn, projectionInv, origin.xyz);

	o_color = vec4(vec3(occlusion), 1.0);
}
