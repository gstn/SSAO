#version 330

in vec4 v_color;
in vec2 v_uv;
in vec3 v_normal;
in vec3 v_position;
in vec3 v_lightPos;

out vec4 o_color;

uniform vec3 u_LightIntensities;
uniform vec3 u_LightPosition;

void main()
{
	o_color = vec4(-v_position.z / 20, -v_position.z / 20, -v_position.z / 20, 1);
}