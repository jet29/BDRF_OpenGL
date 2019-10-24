#version 330 core
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
}dataIn;

uniform vec3 viewPos;
uniform float shininess;

uniform SpotLightProperties spotLight;

void main() {   

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

    
   fragColor=vec4(lightContribution, 1.f);
}

