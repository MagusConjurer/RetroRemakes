#version 330

precision mediump float;

in vec4 vColor;
in vec2 texCoord;
in vec3 normal;

out vec4 fColor;

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D textureData;
uniform DirectionalLight directionalLight;

void main() {
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    // Value between 0 and 1
    float diffuseFactor = max(dot(normalize(normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    fColor = texture(textureData, texCoord) * (ambientColor + diffuseColor);
}