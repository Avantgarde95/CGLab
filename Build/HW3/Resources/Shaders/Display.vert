#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec3 vertexPosition_model;
layout(location = 1) in vec3 vertexNormal_model;
layout(location = 2) in vec2 vertexTextureUV;

out vec3 fragmentPosition_world;
out vec3 fragmentNormal_world;
out vec2 fragmentTextureUV;

// Calculate the "normal vector" version of the matrix.
// (i.e transpose(inverse(matrix)))
mat4 toNormalMatrix(mat4 matrix) {
    mat4 inverseMatrix = inverse(matrix);

	inverseMatrix[0][3] = 0;
	inverseMatrix[1][3] = 0;
	inverseMatrix[2][3] = 0;

	return transpose(inverseMatrix);
}

void main() {
	vec4 vertexPosition_world = modelMatrix * vec4(vertexPosition_model, 1);
	vec4 vertexPosition_eye = viewMatrix * vertexPosition_world;

	vec4 vertexNormal_world = toNormalMatrix(modelMatrix) * vec4(vertexNormal_model, 1);
	vec4 vertexNormal_eye = toNormalMatrix(viewMatrix) * vertexNormal_world;

	// .vert -> GL
	gl_Position = projectionMatrix * vertexPosition_eye;

	// .vert -> .frag
	fragmentPosition_world = vertexPosition_world.xyz;
	fragmentNormal_world = vertexNormal_world.xyz;
	fragmentTextureUV = vertexTextureUV;
}
