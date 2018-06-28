#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
// Interpolated values from the vertex shaders
in vec3 fragmentColor;

// Ouput data
out vec3 color;

uniform vec3 uLight;

void main(){
	// prepare
	vec3 toLight = normalize(uLight - fragmentPosition);
	vec3 normal = normalize(fragmentNormal);

	// diffuse
	float diffuse = max(0.0, dot(normal, toLight));

	// blinn-phong
	float lambertian = max(dot(toLight, normal), 0.0);
	float specular = 0.0;
	vec3 specColor = vec3(1.0, 1.0, 1.0);
	
	if (lambertian > 0.0) {
		float shininess = 50.0;
		vec3 viewDir = normalize(-fragmentPosition);
		vec3 halfDir = normalize(toLight + viewDir);
		float specAngle = max(dot(halfDir, normal), 0.0);
		specular = pow(specAngle, shininess);
	}

	// final color
	vec3 intensity = fragmentColor * diffuse * lambertian + specular * specColor;
	color = pow(intensity, vec3(1.0/2.2)); // gamma correction
}