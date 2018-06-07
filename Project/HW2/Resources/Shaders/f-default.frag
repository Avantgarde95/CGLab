#version 330 core

uniform sampler2D u_textureUnit;

in vec3 f_vertexColor;
in vec2 f_vertexUV;

layout(location = 0) out vec3 r_color;

void main() {
	r_color = f_vertexColor * texture(u_textureUnit, f_vertexUV).rgb;
}
