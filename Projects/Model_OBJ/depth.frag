#version 330

uniform vec3 pixelSize;
uniform int u_kernelSize;
uniform sampler2D u_depthTexture;
uniform vec3 u_kernel[20];
uniform sampler2D u_rotationTexture;

out vec4 o_color;

void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize.xy;
	vec3 depth = (vec3(1)-texture2D(u_depthTexture, pos).rgb)*10;
	o_color = vec4(depth, 1.0);
}