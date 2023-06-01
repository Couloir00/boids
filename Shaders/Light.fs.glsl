#version 330
//variables d'entrees
in vec3 vViewPosition;
in vec3 vWorldPosition;
in vec3 vViewNormal;
in vec3 vWorldNormal;
in vec2 vTexCoords;
in vec3 vColor;
in vec4 vFragPosLightSpace;

//waiting for object materials 
// uniform vec3 uKd;
// uniform vec3 uKs;
// uniform float uShininess;

//light infos
const int MAX_LIGHTS = 10;
// uniform vec3 uLightPos_vs[MAX_LIGHTS];
uniform vec3 uLightPos_ws[MAX_LIGHTS];

uniform sampler2D shadowMap;

// uniform vec3 uLightDir_vs[MAX_LIGHTS];
uniform vec3 uLightDir_ws[MAX_LIGHTS];

uniform vec3 uLightColor[MAX_LIGHTS];
uniform float uLightIntensity[MAX_LIGHTS];
uniform int uLightType[MAX_LIGHTS];

//fog
uniform bool uEnableFog;
uniform float uFogIntensity;
uniform float uRed;
uniform float uGreen;
uniform float uBlue;


//variable de sortie
out vec4 fFragColor;

// Texture
uniform bool uUseTexture;
uniform sampler2D uTexture;

vec3 applyFog(in vec3 rgb){
    if(!uEnableFog){
        return rgb;
    }
    float a = uFogIntensity;
    float b= 0.07f;
    float distance = distance(vViewPosition, vWorldPosition);
    vec3 rayOri = vViewPosition;
    vec3 rayDir = normalize(- vViewPosition);
    float fogAmount = (a / b) * exp(-rayOri.y * b) * (1.0 - exp(-distance * rayDir.y * b)) / rayDir.y;

    vec3 fogColor = vec3(uRed, uGreen, uBlue);
    return mix(rgb, fogColor, fogAmount);
}

vec3 ponctualBlinnPhong(int lightIndex){

    vec3 N = normalize(vViewNormal);

    vec3 wi= normalize(uLightPos_ws[lightIndex]-vWorldPosition);

    float d = distance(vWorldPosition, uLightPos_ws[lightIndex]);

    vec3 Li= (vec3(uLightIntensity[lightIndex])/(d*d));

    vec3 w0 = (normalize(-vWorldPosition));

    vec3 halfVector = (w0+wi)/2.f;

    return Li*uLightColor[lightIndex]*(dot(wi, N));

    // return Li*(uKd*(dot(wi, N))+uKs*pow(dot(halfVector,N),uShininess));
}

vec3 blinnPhong(int lightIndex){
    vec3 wi= normalize(uLightDir_ws[lightIndex]);
    vec3 Li= vec3(uLightIntensity[lightIndex]);
    vec3 w0 = (normalize(-vWorldPosition));
    vec3 N = normalize(vViewNormal);
    vec3 halfVector = (w0+wi)/2.f;
    return Li*uLightColor[lightIndex]*(dot(wi, N));
}

float calculateShadow(vec4 FragPosLightSpace){
    vec3 projCoords = (FragPosLightSpace.xyz) / (FragPosLightSpace.w);
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap,projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05*(1.0 - dot(vViewNormal,uLightDir_ws[0])),0.005);

    //PCF

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap,0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap,projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0){
        shadow = 0.0;
    }

    shadow = 0.0;
    vec3 normal = normalize(vViewNormal);
    vec3 lightDir = normalize(uLightPos_ws[0]-vViewPosition);
    bias = max(0.05*(1.0-dot(normal,lightDir)),0.02);
    closestDepth = texture(shadowMap,projCoords.xy).r;
    currentDepth = projCoords.z;
    shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if(projCoords.z > 1.0){
        shadow = 0.0;
    }
    return shadow;
}

void main() {
    // final color
    vec4 color = vec4(vColor, 1.0);
    vec4 ambient = vec4((0.3 * uLightColor[0]),1.0);
    vec3 sumLights = vec3(0.0);
    //float shadow = 0.0;

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
        //shadow += calculateShadow(vFragPosLightSpace,i);
    }
    //attenue les reflets moches mais attenue aussi l'intensité de la lumière
    sumLights= clamp(sumLights, 0.3, 100.0);
    
    float shadow = calculateShadow(vFragPosLightSpace);
    vec4 lightScene = ((1.0-shadow)*(ambient + vec4(sumLights,1.0)))*color;
    vec3 foggedColor = applyFog(lightScene.rgb);
    fFragColor = vec4(foggedColor, lightScene.a);
}