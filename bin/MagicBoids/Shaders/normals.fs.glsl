#version 330

//input
in vec3 vViewPosition;
in vec3 vViewNormal;
in vec2 vTexCoords;
in vec3 vColor;

//output the color

out vec4 fFragColor;

// Texture
uniform bool uUseTexture;
uniform sampler2D uTexture;

void main(){
  
    // final color
    vec4 color = vec4(vColor, 1.0);

    if (uUseTexture){
        color *= texture(uTexture, vTexCoords);
    }
    fFragColor = color;
    //fFragColor = vec4(0.88f, 0.07f, 0.07f, 1.0f);

}