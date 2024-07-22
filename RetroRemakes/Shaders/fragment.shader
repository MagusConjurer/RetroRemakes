#version 330

precision mediump float;

in vec4 vColor;
in vec2 texCoord;

out vec4 fColor;

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
};

uniform sampler2D textureData;
uniform DirectionalLight directionalLight;

void main() {
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
    fColor = texture(textureData, texCoord) * vColor * ambientColor;
}