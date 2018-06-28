#version 330 core

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

layout(location = 0) in vec3 v_vertexPosition;
layout(location = 1) in vec3 v_vertexNormal;
layout(location = 2) in vec3 v_vertexColor;
layout(location = 3) in vec2 v_vertexUV;

out vec3 f_vertexPosition;
out vec3 f_vertexNormal;
out vec3 f_vertexColor;
out vec2 f_vertexUV;

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
	vec4 vertexPosition_world = u_modelMatrix * vec4(v_vertexPosition, 1);
	vec4 vertexPosition_eye = u_viewMatrix * vertexPosition_world;
	
	vec4 vertexNormal_world = toNormalMatrix(u_modelMatrix) * vec4(v_vertexNormal, 1);
	vec4 vertexNormal_eye = toNormalMatrix(u_viewMatrix) * vertexNormal_world;

	// .vert -> GL
	gl_Position = u_projectionMatrix * vertexPosition_eye;

	// .vert -> .frag
	f_vertexPosition = vertexPosition_world.xyz;
	f_vertexNormal = vertexNormal_world.xyz;
	f_vertexColor = v_vertexColor;
	f_vertexUV = v_vertexUV;
}
