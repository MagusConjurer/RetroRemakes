#version 330

precision mediump float;

in vec4 vColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 fColor;

const int MAX_POINT_LIGHTS = 3;

struct Light {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct PointLight {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct Material {
  float specularIntensity;
  float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D textureData;
uniform Material material;
uniform vec3 eyePosition;

vec4 CalculateLightByDirection(Light light, vec3 direction) {
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    // Value between 0 and 1
    float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if(diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(eyePosition - fragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return ambientColor + diffuseColor + specularColor;
}

vec4 CalculateDirectionalLight() {
    return CalculateLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalculatePointLights() {
    vec4 totalColor = vec4(0,0,0,0);

    for(int i = 0; i < pointLightCount; i++) {
        vec3 direction = fragPos - pointLights[i].position;
        float distance = length(direction);
        direction = normalize(direction);

        vec4 currentColor = CalculateLightByDirection(pointLights[i].base, direction);
        // ax^2 + bx + c
        float attenuation = pointLights[i].exponent * distance * distance + pointLights[i].linear * distance + pointLights[i].constant;

        totalColor += currentColor / attenuation;
    }

    return totalColor;
}

void main() {
    vec4 finalColor = CalculateDirectionalLight();
    finalColor += CalculatePointLights();

    fColor = texture(textureData, texCoord) * finalColor;
}