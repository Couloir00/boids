#version 330

//input
in vec3 vViewPosition;
in vec3 vViewNormal;
in vec2 vTexCoords;

//output the color

out vec4 fFragColor;

void main(){
    fFragColor = vec4(vTexCoords, 1.0f, 1.0f);
}