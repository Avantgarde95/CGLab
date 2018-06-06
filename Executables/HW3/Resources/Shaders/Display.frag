#version 330 core

uniform sampler2D textureUnit;
uniform sampler2D depthMapUnit;
uniform int resolution;

in vec2 fragmentTextureUV;

layout(location = 0) out vec3 fragmentColor;

vec3 getDepthColor(vec2 uv) {
    float depth = texture(depthMapUnit, uv).r;

    return vec3(depth, depth, depth) * vec3(1.0, 0.7, 1.0);
}

// Use Sobel edge detection to calculate the edge.
vec3 calcEdge(vec2 uv) {
    float dx = 0.004;
    float dy = 0.004;

    vec3 a[9] = {
        getDepthColor(uv + vec2(-dx, -dy)),
        getDepthColor(uv + vec2(0.0, -dy)),
        getDepthColor(uv + vec2(+dx, -dy)),
        getDepthColor(uv + vec2(-dx, 0.0)),
        getDepthColor(uv + vec2(0.0, 0.0)),
        getDepthColor(uv + vec2(+dx, 0.0)),
        getDepthColor(uv + vec2(-dx, +dy)),
        getDepthColor(uv + vec2(0.0, +dy)),
        getDepthColor(uv + vec2(+dx, +dy))
    };

    vec3 gX = a[2] + 2.0 * a[5] + a[8] - a[0] - 2.0 * a[3] - a[6];
    vec3 gY = a[0] + 2.0 * a[1] + a[2] - a[6] - 2.0 * a[7] - a[8];

    return sqrt((gX * gX) + (gY * gY));
}

// Use floor function to achieve pixel art effect.
vec2 calcPixelArtUV(vec2 uv) {
    return vec2(
        floor(uv.x * resolution) / (resolution * 1.0),
        floor(uv.y * resolution) / (resolution * 1.0)
    );
}

void main() {
    vec2 uv = calcPixelArtUV(fragmentTextureUV);

	fragmentColor = texture(textureUnit, calcPixelArtUV(uv)).rgb + calcEdge(uv);
}
