#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;

// Values that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform int frameCount;

void main(){	
	// Output position of the vertex, in clip space : MVP * position
	// 4. How To Deal With Our Data In the Shader?
	vec4 position = projectionMatrix * viewMatrix * modelMatrix
	              * vec4(vertexPosition_modelspace, 1);

	position += sin(frameCount / 2000.0f * 3.141592f);

	gl_Position = position;

	// The color of each vertex will be interpolated
	// to produce the color of each fragment
	fragmentColor = vertexColor;
}

