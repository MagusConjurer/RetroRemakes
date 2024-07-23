#version 330

precision mediump float;

in vec4 vColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 fColor;

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material {
  float specularIntensity;
  float shininess;
};

uniform sampler2D textureData;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 eyePosition;


void main() {
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    // Value between 0 and 1
    float diffuseFactor = max(dot(normalize(normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if(diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(eyePosition - fragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    fColor = texture(textureData, texCoord) * (ambientColor + diffuseColor + specularColor);
}