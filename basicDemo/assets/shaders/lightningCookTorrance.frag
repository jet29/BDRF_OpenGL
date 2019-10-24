#version 330 core

#define PI 3.14159265

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
    vec3 normal2;
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

uniform float roughness = 0.3;
uniform float intensity = 1;
uniform float reflectance = 0.8; //reflectance factor


vec3 calcDirLightContribution()
{
	float k = .2;
	vec3 normal=normalize(dataIn.normal);
	// Direction to the light (Directional Light)
    vec3 lightDir=normalize(-dirLight.direction);
	// Vector from the vertex to the camera
    vec3 viewDir=normalize(viewPos-dataIn.vertexPos);


	float NdotL = max(0, dot(normal, lightDir));
	float Rs = 0.0;
	if (NdotL > 0) 
	{
		vec3 H = normalize(lightDir + viewDir);
		float NdotH = max(0, dot(normal, H));
		float NdotV = max(0, dot(normal, viewDir));
		float VdotH = max(0, dot(lightDir, H));

		// Fresnel reflectance
		float F = pow(1.0 - VdotH, 5.0);
		F *= (1.0 - reflectance);
		F += reflectance;

		// Microfacet distribution by Beckmann
		float m_squared = roughness * roughness;
		float r1 = 1.0 / (4.0 * m_squared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (m_squared * NdotH * NdotH);
		float D = r1 * exp(r2);

		// Geometric shadowing
		float two_NdotH = 2.0 * NdotH;
		float g1 = (two_NdotH * NdotV) / VdotH;
		float g2 = (two_NdotH * NdotL) / VdotH;
		float G = min(1.0, min(g1, g2));

		Rs = (F * D * G) / (PI * NdotL * NdotV);
	}
	return dirLight.color.diffuse * NdotL + dirLight.color.specular * NdotL * (k + Rs * (1.0 - k));
}

vec3 calcPointLightContribution(PointLightProperties pointLight)
{
	float k = .2;

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


	// Vector from the vertex to the camera
    vec3 viewDir=normalize(viewPos-dataIn.vertexPos);


	float NdotL = max(0, dot(normal, lightDir));
	float Rs = 0.0;
	if (NdotL > 0) 
	{

		vec3 H = normalize(lightDir + viewDir);
		float NdotH = max(0, dot(normal, H));
		float NdotV = max(0, dot(normal, viewDir));
		float VdotH = max(0, dot(lightDir, H));

		// Fresnel reflectance
		float F = pow(1.0 - VdotH, 5.0);
		F *= (1.0 - reflectance);
		F += reflectance;

		// Microfacet distribution by Beckmann
		float m_squared = roughness * roughness;
		float r1 = 1.0 / (4.0 * m_squared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (m_squared * NdotH * NdotH);
		float D = r1 * exp(r2);

		// Geometric shadowing
		float two_NdotH = 2.0 * NdotH;
		float g1 = (two_NdotH * NdotV) / VdotH;
		float g2 = (two_NdotH * NdotL) / VdotH;
		float G = min(1.0, min(g1, g2));

		Rs = (F * D * G) / (PI * NdotL * NdotV);
	}
	return diffuse + pointLight.color.specular * attenuation *  NdotL * (k + Rs * (1.0 - k));
}

vec3 calcSpotLightContribution()
{
	float k = .2;

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

	// Vector from the vertex to the camera
    vec3 viewDir=normalize(viewPos-dataIn.vertexPos);


	float NdotL = max(0, dot(normal, lightDir));
	float Rs = 0.0;
	if (NdotL > 0) 
	{

		vec3 H = normalize(lightDir + viewDir);
		float NdotH = clamp(dot(normal, H), 0.0f, 1.0f);
		float NdotV = clamp(dot(normal, viewDir), 0.0f ,1.0f);
		float VdotH = clamp(dot(lightDir, H), 0.0f, 1.0f);

		// Fresnel reflectance
		float F = pow(1.0 - VdotH, 5.0);
		F *= (1.0 - reflectance);
		F += reflectance;

		// Microfacet distribution by Beckmann
		float m_squared = roughness * roughness;
		float r1 = 1.0 / (4.0 * m_squared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (m_squared * NdotH * NdotH);
		float D = r1 * exp(r2);

		// Geometric shadowing
		float two_NdotH = 2.0 * NdotH;
		float g1 = (two_NdotH * NdotV) / VdotH;
		float g2 = (two_NdotH * NdotL) / VdotH;
		float G = min(1.0, min(g1, g2));

		Rs = (F * D * G) / (PI * NdotL * NdotV);
	}
	return diffuse + spotLight.color.specular * attenuation * intensity * NdotL * (k + Rs * (1.0 - k));
}


void main()
{
	

	vec3 lightContribution = vec3(0,0,0);

	if(isActiveDirLight) lightContribution += calcDirLightContribution();

    if(isActivePointLight1) lightContribution+=calcPointLightContribution( pointLights[0] );

    if(isActivePointLight2) lightContribution+=calcPointLightContribution( pointLights[1] );

    if(isActiveSpotLight) lightContribution += calcSpotLightContribution();


	fragColor = /*texture(ourTexture, dataIn.uv) * */vec4(lightContribution, 1.0f );

	

}
