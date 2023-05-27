#version 330
layout (location = 0) in vec3 aPosition;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 model;
uniform mat4 LightSpaceMatrix;

void main(){
    gl_Position = LightSpaceMatrix * model * vec4(aPosition,1.0);
}