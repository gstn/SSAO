#version 150
in vec3 N;
in vec3 v;

in vec3 v_Normal;
in vec3 v_Vertex;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;

uniform vec3 u_LightPosition;
uniform vec3 u_LightIntensities;

out vec4 finalColor;

void main(void)
{
	mat3 normalMatrix = transpose(inverse(mat3(u_ModelMatrix)));
	vec3 normal = normalize(normalMatrix * v_Normal);

	vec3 fragPosition = vec3(u_ModelMatrix * vec4(v_Vertex,1));

	vec3 surfaceToLight = u_LightPosition - v_Vertex;

	float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
	brightness = clamp(brightness, 0, 1);

	//finalColor = brightness * vec4(u_LightIntensities, 1.0) * vec4(0.4,0.4,0.4, 1.0);
	finalColor = vec4(normal,1.0);
	//vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
	//vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);  
	//Idiff = clamp(Idiff, 0.0, 1.0); 

	//gl_FragColor = Idiff;
}