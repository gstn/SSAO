#version 330

uniform vec3 pixelSize;
uniform sampler2D texture;
uniform int isVertical;

out vec4 o_color;

void main()
{
	vec4 result;
	vec2 pos = gl_FragCoord.xy * pixelSize.xy;

	float values[9];

	values[0] = 0.05;
	values[1] = 0.09;
	values[2] = 0.11;
	values[3] = 0.15;
	values[4] = 0.2;
	values[5] = 0.15;
	values[6] = 0.11;
	values[7] = 0.09;
	values[8] = 0.05;
	
	if(isVertical == 1) {

		vec2 curSamplePos = vec2(pos.x,pos.y - 4.0 * pixelSize.y);
		for(int i=0;i<9;i++) {
			result += texture2D(texture, curSamplePos) * values[i];
			curSamplePos.y += pixelSize.y;
		}
	} else {

		vec2 curSamplePos = vec2(pos.x - 4.0 * pixelSize.x, pos.y);
		for(int i=0;i<9;i++) {
			result += texture2D(texture, curSamplePos) * values[i];
			curSamplePos.x += pixelSize.x;
		}
	}

	o_color = vec4(result.xyz, 1.0);
}