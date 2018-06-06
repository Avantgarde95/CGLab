#version 330 core

uniform mat4 modelMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

layout(location = 0) in vec3 vertexPosition_model;
layout(location = 1) in vec3 vertexNormal_model;
layout(location = 2) in vec2 vertexTextureUV;

out vec3 fragmentPosition_world;

void main() {
	vec4 vertexPosition_world = modelMatrix * vec4(vertexPosition_model, 1);

	// .vert -> GL
	gl_Position = lightProjectionMatrix * lightViewMatrix * vertexPosition_world;

	// .vert -> .frag
	fragmentPosition_world = vertexPosition_world.xyz;
}
