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

struct PointLightProperties {
   vec3 position;
    LightColor color;
    Attenuation attenuation;
};

in Data{
    vec3 vertexPos;
    vec3 normal;
}dataIn;

uniform vec3 viewPos;
uniform float shininess;

uniform PointLightProperties light;

void main() {   


   vec3 normal=normalize(dataIn.normal);
   // Distance from the vertex to the light
   float distance = length(light.position-dataIn.vertexPos);
   // Attenuation
   float attenuation = 1.0f / (light.attenuation.constant +
  	light.attenuation.linear*distance +
  	light.attenuation.quadratic*(distance*distance));

   vec3 ambient=light.color.ambient * attenuation;
   vec3 lightContribution=ambient;
   // Direction to the light from the vertex
   vec3 lightDir=normalize(light.position-dataIn.vertexPos);
   // Lambert cos(angle(Normal, Light))
   float diff=max(dot(normal,lightDir),0.f);
   vec3 diffuse=diff * light.color.diffuse * attenuation ;
   lightContribution=lightContribution+diffuse;

   if(diff>0.f){
       // Vector from the vertex to the camera
        vec3 viewDir=normalize(viewPos-dataIn.vertexPos);
       // Blinn-Phong
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec=pow(max(dot(normal,halfwayDir),0.f),shininess);
        vec3 specular=light.color.specular*spec * attenuation;
        lightContribution=lightContribution+specular;
    }

   fragColor=vec4(lightContribution, 1.f);

}
