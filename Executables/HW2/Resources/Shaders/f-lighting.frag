#version 330 core

// See Engine/Material.hpp.
uniform struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} u_material;

// See Engine/Light.hpp.
uniform struct Light {
	int type;
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float angle;
	float attenuation;
} u_lightList[5];

uniform sampler2D u_textureUnit;
uniform sampler2D u_normalMapUnit;

in vec3 f_vertexPosition;
in vec3 f_vertexNormal;
in vec3 f_vertexColor;
in vec2 f_vertexUV;

layout(location = 0) out vec3 r_color;

// Normal mapping.
vec3 calcNormal() {
	vec3 normalMapColor = texture(u_normalMapUnit, f_vertexUV).rgb;
	vec3 normalDiff = normalize(normalMapColor * 2.0 - 1.0);

	return normalize(f_vertexNormal + normalDiff);
}

// Calculate the attenuation for the light.
float calcAttenuation(Light light) {
	// Directional light.
	if (light.type == 0) {
		return 1.0;
	}

	// Point light.
	float distanceToLight = length(light.position - f_vertexPosition);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2.0));

	if (light.type == 1) {
		return attenuation;	
	}

	// Spotlight.
	vec3 toLight = normalize(light.position - f_vertexPosition);
	float vertexAngle = degrees(acos(dot(-toLight, normalize(light.direction))));

	if (vertexAngle > light.angle) {
		attenuation = 0.0;
	}

	return attenuation;
}

// Calculate the ambient term.
vec3 calcAmbient(Light light) {
	return u_material.ambient * light.ambient;
}

// Calculate the diffuse term.
vec3 calcDiffuse(Light light, vec3 myNormal) {
	vec3 toLight = normalize(light.position - f_vertexPosition);
	float lambertian = max(dot(myNormal, toLight), 0.0);
	
	return u_material.diffuse * light.diffuse * lambertian;
}

// Calculate the specular term.
vec3 calcSpecular(Light light, vec3 myNormal) {
	vec3 toLight = normalize(light.position - f_vertexPosition);

	if (dot(myNormal, toLight) <= 0.0) {
		return 0.0;
	}

	// Gaussian distribution.
	vec3 toEye = -f_vertexPosition;
	vec3 halfway = normalize(toLight + toEye);
	float angle = acos(max(dot(myNormal, halfway), 0.0));
	float smoothness = 0.7;
	float factor = exp(-pow(angle / smoothness, 2.0));

	return u_material.specular * light.specular * factor;
}

void main() {
	vec3 myNormal = calcNormal();
	vec3 intensity = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < 5; i++) {
		Light light = u_lightList[i];

		if (light.type >= 0) {
			float attenuation = calcAttenuation(light);
			
			intensity += f_vertexColor * calcAmbient(light);
			intensity += attenuation * calcDiffuse(light, myNormal);
			intensity += attenuation * calcSpecular(light, myNormal);
		}
	}

	r_color = texture(u_textureUnit, f_vertexUV).rgb * intensity;
}
