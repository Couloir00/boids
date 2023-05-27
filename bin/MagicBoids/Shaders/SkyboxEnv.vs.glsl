#version 330
layout (location = 0) in vec3 aPosition;

out vec3 TexCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
    TexCoords = aPosition;
    vec4 position = projectionMatrix * viewMatrix * vec4 (aPosition,1.0);
    gl_Position = position.xyww;
}