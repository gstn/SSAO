#version 330

uniform vec3 pixelSize;
uniform sampler2D u_texture;
uniform int u_blurSize = 4;

in vec2 v_uv;
out vec4 o_color;

void main()
{
	float result;
	vec2 pos = v_uv * pixelSize.xy;

	vec2 hlim = vec2(-u_blurSize);
	hlim = hlim * 0.5 + 0.5;

	
	for(int i = 0; i < u_blurSize; ++i) {
		for(int j = 0; j < u_blurSize; ++j) {
			vec2 offset = (hlim + vec2(i, j)) * pixelSize.xy;
			result += texture2D(u_texture, v_uv + offset).r;
		}
	}
	
	result /= u_blurSize * u_blurSize;

	o_color = vec4(vec3(result), 1.0);
}