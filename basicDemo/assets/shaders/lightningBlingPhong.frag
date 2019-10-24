#version 330 core
#define NUM_POINTLIGHT 2
#define NUM_SPOTLIGHT 1
#define NUM_DIRLIGHT 1


out vec4 fragColor;

struct LightColor {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Attenuation{
    float constant;
    float linear;
    float quadratic;
};

struct PointLightProperties {
   vec3 position;
    LightColor color;
    Attenuation attenuation;
};

struct DirectionalLightProperties {
   vec3 direction;
   LightColor color;
};

struct SpotLightProperties{
    vec3 position;
    vec3 direction;
    LightColor color;
    float cutOff;
    float outerCutOff;
    Attenuation attenuation;
};



in Data{
    vec3 vertexPos;
    vec3 normal;
    vec2 uv;
}dataIn;

uniform vec3 viewPos;
uniform float shininess;
uniform sampler2D ourTexture;

//IS ACTIVE
uniform bool isActiveDirLight;
uniform bool isActiveSpotLight;
uniform bool isActivePointLight1;
uniform bool isActivePointLight2;

uniform PointLightProperties pointLights[NUM_POINTLIGHT];
uniform DirectionalLightProperties dirLight;
uniform SpotLightProperties spotLight;

vec3 calcPointLightContribution(PointLightProperties pointLight){

    vec3 normal=normalize(dataIn.normal);
   // Distance from the vertex to the light
   float distance = length(pointLight.position-dataIn.vertexPos);
   // Attenuation
   float attenuation = 1.0f / (pointLight.attenuation.constant +
  	pointLight.attenuation.linear*distance +
  	pointLight.attenuation.quadratic*(distance*distance));

   vec3 ambient=pointLight.color.ambient * attenuation;
   vec3 lightContribution=ambient;
   // Direction to the light from the vertex
   vec3 lightDir=normalize(pointLight.position-dataIn.vertexPos);
   // Lambert cos(angle(Normal, Light))
   float diff=max(dot(normal,lightDir),0.f);
   vec3 diffuse=diff * pointLight.color.diffuse * attenuation ;
   lightContribution=lightContribution+diffuse;

   if(diff>0.f){
       // Vector from the vertex to the camera
        vec3 viewDir=normalize(viewPos-dataIn.vertexPos);
       // Blinn-Phong
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec=pow(max(dot(normal,halfwayDir),0.f),shininess);
        vec3 specular=pointLight.color.specular*spec * attenuation;
        lightContribution=lightContribution+specular;
    }

    return lightContribution;
}

vec3 calcDirLightContribution(){

    vec3 normal=normalize(dataIn.normal);
    vec3 ambient=dirLight.color.ambient;
   
    vec3 lightContribution=ambient;

    // Direction to the light (Directional Light)
    vec3 lightDir=normalize(-dirLight.direction);

    // Lambert cos(angle(Normal, Light))
    float diff=max(dot(normal,lightDir),0.f);
    vec3 diffuse=diff*dirLight.color.diffuse;
    lightContribution=lightContribution+diffuse;

    if(diff>0.f){

        // Vector from the vertex to the camera
        vec3 viewDir=normalize(viewPos-dataIn.vertexPos);
        // Blinn-Phong
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec=pow(max(dot(normal,halfwayDir),0.f),shininess);
        vec3 specular=dirLight.color.specular*spec;
        lightContribution=lightContribution+specular;

        
    }

    return lightContribution;
}

vec3 calcSpotLightContribution(){

    vec3 normal=normalize(dataIn.normal);
   // Distance from the vertex to the light
   float distance=length(spotLight.position-dataIn.vertexPos);
   // Attenuation
   float attenuation = 1.0f / (spotLight.attenuation.constant +
  	spotLight.attenuation.linear*distance +
  	spotLight.attenuation.quadratic*(distance*distance));

   vec3 ambient=spotLight.color.ambient * attenuation;
   vec3 lightContribution=ambient;
   // Direction to the light from the vertex
   vec3 lightDir=normalize(spotLight.position-dataIn.vertexPos);
   float theta=dot(lightDir,normalize(-spotLight.direction));
   float epsilon=spotLight.cutOff-spotLight.outerCutOff;
   float intensity=clamp((theta-spotLight.outerCutOff)/epsilon,0.,1.);

   // Lambert cos(angle(Normal, Light))
   float diff=max(dot(normal,lightDir),0.f);
   vec3 diffuse=diff * spotLight.color.diffuse * attenuation *intensity;
   lightContribution=lightContribution+diffuse;

   if(diff>0.f){
       // Vector from the vertex to the camera
        vec3 viewDir=normalize(viewPos-dataIn.vertexPos);
       // Blinn-Phong
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec=pow(max(dot(normal,halfwayDir),0.f),shininess);
        vec3 specular=spotLight.color.specular*spec*attenuation*intensity;
        lightContribution=lightContribution+specular;
    }

    
   return lightContribution;
}


void main() {   

    
    vec3 lightContribution = vec3(0,0,0);
    
    if(isActivePointLight1) lightContribution+=calcPointLightContribution( pointLights[0] );

    if(isActivePointLight2) lightContribution+=calcPointLightContribution( pointLights[1] );
    
    if(isActiveDirLight) lightContribution+=calcDirLightContribution();

    if(isActiveSpotLight) lightContribution+=calcSpotLightContribution();
    

    fragColor = texture(ourTexture, dataIn.uv) * vec4(lightContribution,1.f);
    

    //fragColor = vec4(dataIn.normal, 1.f);

}

