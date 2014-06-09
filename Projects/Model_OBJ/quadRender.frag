uniform sampler2D texture;
uniform vec3 pixelSize; //==x: 1/640,y=1/480
varying vec2 outUV;

void main()
{
	vec2 pos=gl_FragCoord.xy * pixelSize.xy;
	gl_FragColor=texture2D(texture,pos) + vec4(1,0,0,1);
	//gl_FragColor=vec4(1,0,0,1);
}
