uniform sampler2D texture;
uniform vec3 pixelSize;

void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize.xy;
	gl_FragColor = texture2D(texture, pos);
}
