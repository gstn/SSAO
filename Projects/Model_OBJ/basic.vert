#version 150
out vec3 N;
out vec3 v;


out vec3 v_Normal;
out vec3 v_Vertex;

attribute vec4 a_Position;
attribute vec4 a_Normal;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;

uniform vec3 u_LightPosition;
uniform vec3 u_LightIntensities;

void main(void)
{

	//v = vec3(gl_ModelViewMatrix * a_Position);       
	//N = normalize(gl_NormalMatrix * gl_Normal);
	v_Normal = a_Normal.xyz;
	v_Vertex = a_Position.xyz;

	//gl_PointSize = a_Position.z * 2.0f + 2.0f;
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * a_Position;
	//gl_Position = gl_ModelViewProjectionMatrix * a_Position;

}
          