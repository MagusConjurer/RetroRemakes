#version 330							    

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec4 col;
layout (location = 3) in vec3 norm;

out vec4 vColor;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vColor = col;

    texCoord = tex;

    // Account for non uniform scaling and remove unneeded transform data
    normal = mat3(transpose(inverse(model))) * norm;

    // Position of the lit point in world coordinates
    fragPos = (model * vec4(pos, 1.0)).xyz;
}