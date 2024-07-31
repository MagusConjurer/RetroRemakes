#version 330

precision mediump float;

in vec4 vColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 directionalLightSpacePos;

out vec4 fColor;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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

struct SpotLight {
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material {
  float specularIntensity;
  float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D textureData;
uniform sampler2D directionalShadowMap;
uniform Material material;
uniform vec3 eyePosition;

float CalcDirectionalShadowFactor(DirectionalLight light) {
    // calculate in normalized device coordinates between -1, 1
    vec3 projectedCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
    // Shift to be between 0, 1 that depth map expects
    projectedCoords = (projectedCoords * 0.5) + 0.5;

    float current = projectedCoords.z;

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(light.direction);
    float bias = max(0.05 * (1.0 - dot(n,lightDir)), 0.005);

    // Get average shadow color of the 3x3 square with this texel in the middle
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcfDepth = texture(directionalShadowMap, projectedCoords.xy + vec2(x,y) * texelSize).r;
            shadow += current - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    // Handle when it is past the far plane
    if (projectedCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

vec4 CalculateLightByDirection(Light light, vec3 direction, float shadowFactor) {
    vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;

    // Value between 0 and 1
    float diffuseFactor = max(dot(normalize(normal), -normalize(direction)), 0.0);
    vec4 diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
    if(diffuseFactor > 0.0) {
        vec3 fragToEye = normalize(eyePosition - fragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0);
        }
    }

    return ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor);
}

vec4 CalculatePointLight(PointLight pLight) {
    vec3 direction = fragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 currentColor = CalculateLightByDirection(pLight.base, direction, 0.0);
    // ax^2 + bx + c
    float attenuation = pLight.exponent * distance * distance + pLight.linear * distance + pLight.constant;

    return currentColor / attenuation;
}

vec4 CalculateSpotLight(SpotLight sLight) {
    vec3 rayDirection = normalize(fragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);

    if(slFactor > sLight.edge) {
        vec4 color = CalculatePointLight(sLight.base);

        return color * (1.0 - ((1.0 - slFactor) * (1.0/(1.0 - sLight.edge))));
    } else {
        return vec4(0, 0, 0, 0);
    }
}

vec4 CalculateDirectionalLight() {
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
    return CalculateLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalculatePointLights() {
    vec4 totalColor = vec4(0,0,0,0);

    for(int i = 0; i < pointLightCount; i++) {
        totalColor += CalculatePointLight(pointLights[i]);
    }

    return totalColor;
}

vec4 CalculateSpotLights() {
     vec4 totalColor = vec4(0,0,0,0);

    for(int i = 0; i < spotLightCount; i++) {
        totalColor += CalculateSpotLight(spotLights[i]);
    }

    return totalColor;
}

void main() {
    vec4 finalColor = CalculateDirectionalLight();
    finalColor += CalculatePointLights();
    finalColor += CalculateSpotLights();

    fColor = texture(textureData, texCoord) * finalColor;
}