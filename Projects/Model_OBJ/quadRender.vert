attribute vec4 vertex;
attribute vec2 UV;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;

varying vec2 outUV;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vertex;
	outUV=UV;
}
