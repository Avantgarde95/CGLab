#version 330 core

uniform sampler2D u_textureUnit;

in vec3 f_vertexColor;
in vec2 f_vertexUV;

layout(location = 0) out vec3 r_color;

// Simple single-pass blur.
vec3 calcBlur(float offset, float weight) {
	vec3 color = vec3(0.0, 0.0, 0.0);

	color += texture(u_textureUnit, f_vertexUV + vec2(0.0, offset)).rgb * weight;
	color += texture(u_textureUnit, f_vertexUV - vec2(0.0, offset)).rgb * weight;

	return color;
}

void main() {
	float blurOffset[5] = float[](
		0.0, 0.004, 0.008, 0.012, 0.016
	);

	float blurWeight[5] = float[](
		0.2270270270,
		0.1945945946,
		0.1216216216,
		0.0540540541,
		0.0162162162
	);


	r_color = texture(u_textureUnit, f_vertexUV).rgb * blurWeight[0];

	for (int i = 1; i < 5; i++) {
		r_color += calcBlur(blurOffset[i], blurWeight[i]);
	}
}
