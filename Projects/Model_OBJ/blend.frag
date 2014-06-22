uniform vec3 pixelSize;
uniform sampler2D u_renderTexture;
uniform sampler2D u_ssaoTexture;

void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize;
	vec3 render = vec3(texture2D(u_renderTexture, pos));
	vec3 ssao = vec3(texture2D(u_ssaoTexture, pos));

	gl_FragColor = vec4(ssao * render, 1.0);
}
