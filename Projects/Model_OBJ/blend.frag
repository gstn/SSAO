#version 330

uniform vec3 pixelSize;
uniform sampler2D u_renderTexture;
uniform sampler2D u_ssaoTexture;

out vec4 o_color;

void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize.xy;
	vec3 render = vec3(texture2D(u_renderTexture, pos));
	vec3 ssao = vec3(texture2D(u_ssaoTexture, pos));

	o_color = vec4(ssao * render, 1.0);
}
