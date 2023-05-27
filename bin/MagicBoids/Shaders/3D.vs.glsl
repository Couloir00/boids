#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;
layout(location = 3) in vec3 aVertexColor;

//output variables
out vec3 vViewPosition;
out vec3 vWorldPosition;
out vec3 vViewNormal;
out vec3 vWorldNormal;
out vec2 vTexCoords;
out vec3 vColor;
out vec4 vFragPosLightSpace;

//transformations
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 model;
uniform mat4 LightSpaceMatrix;

void main() {
    //passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1.);
    vec4 vertexNormal = vec4(aVertexNormal, 0.);

    //Calcul des valeurs de sortie
    vWorldPosition = vec3(model*vertexPosition);
    vWorldNormal = vec3(vertexNormal);
    vViewPosition = vec3(uMVMatrix * vertexPosition);
    vViewNormal = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;
    vColor = aVertexColor;
    vFragPosLightSpace = LightSpaceMatrix * model *vertexPosition;
    
    //Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
}