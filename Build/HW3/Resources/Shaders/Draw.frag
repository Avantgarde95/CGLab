#version 330 core

uniform struct Light {
	int type;
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float angle;
	float attenuation;
} lightList[5];

uniform sampler2D textureUnit;
uniform sampler2D shadowMapUnit;
uniform sampler2D brushTextureUnit;
uniform vec3 cameraPosition;
uniform int isSelected;

in vec3 fragmentPosition_world;
in vec3 fragmentNormal_world;
in vec2 fragmentTextureUV;
in vec3 fragmentShadowUVZ;

layout(location = 0) out vec3 fragmentColor;

// Direction to the light.
vec3 toLight(Light light) {
    // Directional light.
    if (light.type == 1) {
        return -light.direction;
    }

    // Otherwise.
    return normalize(light.position - fragmentPosition_world);
}

// Direction to the camera(eye).
vec3 toEye() {
    return normalize(cameraPosition - fragmentPosition_world);
}

float calcAttenuation(Light light) {
	// Directional light.
	if (light.type == 1) {
		return 1.0;
	}

	// Point light.
	float distanceToLight = length(light.position - fragmentPosition_world);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2.0));

	if (light.type == 2) {
		return attenuation;
	}

	// Spotlight.
	float vertexAngle = degrees(acos(dot(-toLight(light), normalize(light.direction))));

	if (vertexAngle > light.angle) {
		attenuation = 0.0;
	}

	return attenuation;
}

vec3 calcAmbient(Light light) {
	return light.ambient;
}

vec3 calcDiffuse(Light light) {
	float lambertian = max(dot(fragmentNormal_world, toLight(light)), 0.0);

	return light.diffuse * lambertian;
}

vec3 calcSpecular(Light light) {
	if (dot(fragmentNormal_world, toLight(light)) <= 0.0) {
		return vec3(0.0, 0.0, 0.0);
	}

	// Gaussian distribution.
	vec3 toEye = normalize(cameraPosition - fragmentPosition_world);
	vec3 halfway = normalize(toLight(light) + toEye());
	float angle = acos(max(dot(fragmentNormal_world, halfway), 0.0));
	float smoothness = 0.7;
	float factor = exp(-pow(angle / smoothness, 2.0));

	return light.specular * factor;
}

float calcShadow() {
    // We give a small bias to solve shadow acne problem.
    float bias = 0.005;

    if (texture(shadowMapUnit, fragmentShadowUVZ.xy).r < fragmentShadowUVZ.z - bias) {
        return 0.2;
    }
    else {
        return 1.0;
    }
}

// Combine the color and the brush texture to achieve brush-like effect.
vec3 applyBrush(vec3 color) {
    return color * texture(brushTextureUnit, fragmentTextureUV).rgb;
}

vec3 calcCrossHatch(vec3 frontColor, vec3 backColor) {
    float diff = 15.0;
    float colorNorm = length(texture(textureUnit, fragmentTextureUV).rgb);
    vec3 resultColor = backColor;

    if (colorNorm < 1.00) {
        if (mod(gl_FragCoord.x + gl_FragCoord.y, diff) == 0.0) {
            resultColor = frontColor;
        }
    }

    if (colorNorm < 0.75) {
        if (mod(gl_FragCoord.x - gl_FragCoord.y, diff) == 0.0) {
            resultColor = frontColor;
        }
    }

    if (colorNorm < 0.50) {
        if (mod(gl_FragCoord.x + gl_FragCoord.y - diff / 2.0, diff) == 0.0) {
            resultColor = frontColor;
        }
    }

    if (colorNorm < 0.3) {
        if (mod(gl_FragCoord.x - gl_FragCoord.y - diff / 2.0, diff) == 0.0) {
            resultColor = frontColor;
        }
    }

    return resultColor;
}

void main() {
    // (1) Calculate the lights.
    vec3 intensity = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < 5; i++) {
		Light light = lightList[i];

		if (light.type > 0) {
			float attenuation = calcAttenuation(light);

			intensity += calcAmbient(light);
			intensity += attenuation * calcDiffuse(light);
			intensity += attenuation * calcSpecular(light);
		}
	}

    // (2) Brush effect + Lighting + Shadow map.
    fragmentColor = applyBrush(texture(textureUnit, fragmentTextureUV).rgb) * intensity * calcShadow();

    // (3) If we selected the current model, apply cross hatching.
	if (isSelected != 0) {
	    fragmentColor = calcCrossHatch(fragmentColor, vec3(1.0, 1.0, 1.0));
	}
}
