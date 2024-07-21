#version 330

precision mediump float;

in vec4 vColor;
in vec2 texCoord;

out vec4 fColor;

uniform sampler2D textureData;

void main() {
    fColor = texture(textureData, texCoord) * vColor;
}