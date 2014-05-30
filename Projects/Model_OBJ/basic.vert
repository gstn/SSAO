#version 330

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;

uniform vec3 u_LightPosition;
uniform vec3 u_LightIntensities;

in vec4 vertex;
in vec3 normal;
in vec4 tangent;
in vec4 color;
in vec2 uv;

out vec4 v_color;
out vec2 v_uv;
out vec3 v_normal;
out vec3 v_position;
out vec3 v_lightPos;

void main()
{
	v_lightPos = (u_ViewMatrix *  vec4(u_LightPosition,1)).xyz;
	v_position = (u_ViewMatrix * u_ModelMatrix * vertex).xyz;
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vertex;
	
	v_color = color;
	v_uv = uv;
	v_normal = mat3(u_ViewMatrix * u_ModelMatrix)*normal;
}