#version 330

//input
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

//output the color

out vec4 fFragColor;

void main(){
    fFragColor = vec4(vTexCoords, 1.0f, 1.0f);
}