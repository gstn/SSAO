#version 330

uniform vec3 pixelSize;
uniform sampler2D u_renderTexture;
uniform sampler2D u_ssaoTexture;

in vec2 v_uv;
out vec4 o_color;

void main()
{
	vec3 render = vec3(texture2D(u_renderTexture, v_uv));
	vec3 ssao = vec3(texture2D(u_ssaoTexture, v_uv));

	o_color = vec4(ssao * render, 1.0);
}
