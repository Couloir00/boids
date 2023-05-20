#version 330
//variables d'entrees
in vec3 vViewPosition;
in vec3 vWorldPosition;
in vec3 vViewNormal;
in vec3 vWorldNormal;
in vec2 vTexCoords;
in vec3 vColor;

//waiting for object materials 
// uniform vec3 uKd;
// uniform vec3 uKs;
// uniform float uShininess;

//light infos
const int MAX_LIGHTS = 10;
// uniform vec3 uLightPos_vs[MAX_LIGHTS];
uniform vec3 uLightPos_ws[MAX_LIGHTS];

// uniform vec3 uLightDir_vs[MAX_LIGHTS];
uniform vec3 uLightDir_ws[MAX_LIGHTS];

uniform vec3 uLightColor[MAX_LIGHTS];
uniform float uLightIntensity[MAX_LIGHTS];
uniform int uLightType[MAX_LIGHTS];


//variable de sortie
out vec4 fFragColor;

// Texture
uniform bool uUseTexture;
uniform sampler2D uTexture;

vec3 ponctualBlinnPhong(int lightIndex){
    float d = distance(vWorldPosition, uLightPos_ws[lightIndex]);
    vec3 wi= normalize(uLightPos_ws[lightIndex]-vWorldPosition);
    vec3 Li= (vec3(uLightIntensity[lightIndex])/(d*d));
    vec3 w0 = (normalize(-vWorldPosition));
    vec3 N = vWorldNormal;
    vec3 halfVector = (w0+wi)/2.f;
    return Li*uLightColor[lightIndex]*(dot(wi, N));
    // return Li*(uKd*(dot(wi, N))+uKs*pow(dot(halfVector,N),uShininess));
}

vec3 blinnPhong(int lightIndex){
    vec3 wi= normalize(uLightDir_ws[lightIndex]);
    vec3 Li= vec3(uLightIntensity[lightIndex]);
    vec3 w0 = (normalize(-vWorldPosition));
    vec3 N = vWorldNormal;
    vec3 halfVector = (w0+wi)/2.f;
    return Li*uLightColor[lightIndex]*(dot(wi, N));
}

void main() {
    // final color
    vec4 color = vec4(vColor, 1.0);
    vec3 sumLights = vec3(0.0);

    if (uUseTexture){
        color *= texture(uTexture, vTexCoords);
    }
    

    for(int i=0; i<MAX_LIGHTS; i++){
        if(uLightType[i] == 0){
            sumLights += ponctualBlinnPhong(i);
        }
        else if(uLightType[i] == 1){
            sumLights += blinnPhong(i);
        }
    }
    fFragColor = vec4(sumLights,1.)*color;
}