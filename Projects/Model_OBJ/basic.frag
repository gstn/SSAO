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
	vec3 vInvlightDir =  normalize( v_position - v_lightPos );
	vec3 E = normalize( -v_position );
	vec3 R = normalize( -reflect( vInvlightDir, v_normal ) );

	//diffuse
	float diffuse = max( dot( v_normal, vInvlightDir ), 0 );
	vec3 vdiffuse = vec3(0,0,1) * clamp( diffuse, 0, 1 );

	//specular
	float spec = pow( max( dot( R, E ), 0 ), 5 );
	vec3 vspec = vec3(1) * clamp( spec, 0, 1 );

	//ambient
	vec3 vambient = vec3( 0.2 );
	
	o_color = vec4( vdiffuse + vspec + vambient, 1 );
}