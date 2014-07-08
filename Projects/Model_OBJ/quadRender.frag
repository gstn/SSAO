uniform sampler2D texture;
uniform vec2 u_pixelSize;

void main()
{
	vec2 pos = gl_FragCoord.xy * u_pixelSize;
	gl_FragColor = texture2D(texture, pos);
}
