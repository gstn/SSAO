#version 330

uniform vec3 pixelSize;
uniform vec3 u_kernel[10];
uniform int u_kernelSize;
uniform float u_radius;
uniform float u_screenW;
uniform float u_screenH;
uniform mat4 u_ProjectionMatrix;
uniform sampler2D u_depthTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_positionTexture;
uniform sampler2D u_noiseTexture;

uniform mat4 u_persMatrix;
uniform float u_near;
uniform float u_far;
uniform vec4 u_frustum;

in vec2 v_uv;

out vec4 o_color;

vec3 calcEye(float depth) {

	vec2 ndc;
	vec3 eye;

	eye.z = u_near * u_far / ((depth * (u_far - u_near)) - u_far);

	ndc = gl_FragCoord.xy * pixelSize.xy * 2.0 - 1.0;

	eye.x = (-ndc.x * eye.z) * (u_frustum.y - u_frustum.x) / (2 * u_near) - eye.z * (u_frustum.y + u_frustum.x) / (2 * u_near);
	eye.y = (-ndc.y * eye.z) * (u_frustum.w - u_frustum.z) / (2 * u_near) - eye.z * (u_frustum.w + u_frustum.z) / (2 * u_near);

	eye.z = -eye.z;

	return eye;
}

void main() {
	float occlusion = 0.0;
	float depth = texture2D(u_depthTexture, v_uv).x;
	vec3 normal = (texture2D(u_normalTexture, v_uv).xyz - 0.5) * 2.0;
	vec3 origin = texture2D(u_positionTexture, v_uv).xyz;

	vec3 position = calcEye(vec3(depth));

	o_color = vec4(position, 1.0);
}
