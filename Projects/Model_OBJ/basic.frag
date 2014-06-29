#version 330

in vec4 v_color;
in vec2 v_uv;
in vec3 v_normal;
in vec3 v_position;
in vec3 v_lightPos;

//out vec4 o_color;
layout(location=0) out vec4 o_color;
layout(location=1) out vec4 o_normal;
layout(location=2) out vec4 o_position;

uniform vec3 u_LightIntensities;
uniform vec3 u_LightPosition;
uniform sampler2D u_texture0;

void main()
{	
	vec3 vInvlightDir =  normalize( v_position - v_lightPos );
	vec3 E = normalize( -v_position );
	vec3 R = normalize( -reflect( vInvlightDir, v_normal ) );

	//diffuse
	float diffuse = max( dot( v_normal, vInvlightDir ), 0 );
	vec3 vdiffuse = vec3(.8) * clamp( diffuse, 0, 1 );

	//specular
	float spec = pow( max( dot( R, E ), 0 ), 1 );
	vec3 vspec = vec3(.1) * clamp( spec, 0, 1 );

	//ambient
	vec3 vambient = vec3( 0.2 );

	//texture
	vec4 vtexture = texture2D(u_texture0, v_uv) + .5;

	vec3 color = (vdiffuse + vspec) * u_LightIntensities;
	
	//o_color = vec4( color + vambient, 1 ) * vtexture;
	o_color = vec4(color + vambient, 1 ) * vtexture;
	o_normal = vec4(v_normal, 1.0);
	o_position = vec4(v_position, 1.0);
}