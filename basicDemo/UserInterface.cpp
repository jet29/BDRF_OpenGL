#include "UserInterface.h"


// Global static pointer used to ensure a single instance of the class.
CUserInterface * CUserInterface::mInterface = NULL;

/**
* Creates an instance of the class
*
* @return the instance of this class
*/
CUserInterface * CUserInterface::Instance()
{
	if (!mInterface)   // Only allow one instance of class to be generated.
		mInterface = new CUserInterface();

	return mInterface;
}

CUserInterface::CUserInterface()
{
	mUserInterface = TwNewBar("Tarea 1");

	pointLight1Translation[0] = 0.0f;
	pointLight1Translation[1] = 4.0f;
	pointLight1Translation[2] = 4.0f;

	pointLight2Translation[0] = 0.0f;
	pointLight2Translation[1] = 4.0f;
	pointLight2Translation[2] = -4.0f;


	g_enablePointLight1 = true;
	g_enablePointLight2 = true;

	//POINT LIGHT 1
	TwAddVarRW(mUserInterface, "PointLight1 Ambient", TW_TYPE_COLOR3F, &pointLight1AColor, " label=' Point Light 1 Ambient' group = 'PointLight1' ");
	TwAddVarRW(mUserInterface, "PointLight1 Diffuse", TW_TYPE_COLOR3F, &pointLight1DColor, " label=' Point Light 1 Diffuse' group = 'PointLight1' ");
	TwAddVarRW(mUserInterface, "PointLight1 Specular", TW_TYPE_COLOR3F, &pointLight1SColor, " label=' Point Light 1 Specular' group = 'PointLight1' ");
	

	TwAddVarRW(mUserInterface, "X PointLight1", TW_TYPE_FLOAT, &pointLight1Translation[0], " group='PointLight1' step=0.01 ");
	TwAddVarRW(mUserInterface, "Y PointLight1", TW_TYPE_FLOAT, &pointLight1Translation[1], " group='PointLight1' step=0.01 ");
	TwAddVarRW(mUserInterface, "Z PointLight1", TW_TYPE_FLOAT, &pointLight1Translation[2], " group='PointLight1' step=0.01 ");

	TwAddVarRW(mUserInterface, "ConstantAtt Light1", TW_TYPE_FLOAT, &pointLight1AttConsColor, " min=0 max=64 step=0.01 label=' ConstantAtt' group = 'PointLight1' ");
	TwAddVarRW(mUserInterface, "LinearAtt Light1", TW_TYPE_FLOAT, &pointLight1AttLiColor, " min=0 max=64 step=0.01 label=' LinearAtt' group = 'PointLight1' ");
	TwAddVarRW(mUserInterface, "QuadraticAtt Light1", TW_TYPE_FLOAT, &pointLight1AttCuaColor, " min=0 max=64 step=0.01 label=' QuadraticAtt' group = 'PointLight1' ");
	TwAddVarRW(mUserInterface, "isActive PointLight1", TW_TYPE_BOOLCPP, &isActivePointLight1, " group = 'PointLight1' ");
	
	//POINT LIGHT 2
	TwAddVarRW(mUserInterface, "PointLight2 Ambient", TW_TYPE_COLOR3F, &pointLight2AColor, " label=' Point Light 2 Ambient' group = 'PointLight2' ");
	TwAddVarRW(mUserInterface, "PointLight2 Diffuse", TW_TYPE_COLOR3F, &pointLight2DColor, " label=' Point Light 2 Diffuse' group = 'PointLight2' ");
	TwAddVarRW(mUserInterface, "PointLight2 Specular", TW_TYPE_COLOR3F, &pointLight2SColor, " label=' Point Light 2 Specular' group = 'PointLight2' ");

	TwAddVarRW(mUserInterface, "X PointLight2", TW_TYPE_FLOAT, &pointLight2Translation[0], " group='PointLight2' step=0.01 ");
	TwAddVarRW(mUserInterface, "Y PointLight2", TW_TYPE_FLOAT, &pointLight2Translation[1], " group='PointLight2' step=0.01 ");
	TwAddVarRW(mUserInterface, "Z PointLight2", TW_TYPE_FLOAT, &pointLight2Translation[2], " group='PointLight2' step=0.01 ");


	TwAddVarRW(mUserInterface, "ConstantAtt Light2", TW_TYPE_FLOAT, &pointLight2AttConsColor, " min=0 max=64 step=0.01 label=' ConstantAtt' group = 'PointLight2' ");
	TwAddVarRW(mUserInterface, "LinearAtt Light2", TW_TYPE_FLOAT, &pointLight2AttLiColor, " min=0 max=64 step=0.01 label=' LinearAtt' group = 'PointLight2' ");
	TwAddVarRW(mUserInterface, "QuadraticAtt Light2", TW_TYPE_FLOAT, &pointLight2AttCuaColor, " min=0 max=64 step=0.01 label=' QuadraticAtt' group = 'PointLight2' ");

	TwAddVarRW(mUserInterface, "isActive PointLight2", TW_TYPE_BOOLCPP, &isActivePointLight2, " group = 'PointLight2' ");

	//DIRECTIONAL LIGHT
	TwAddVarRW(mUserInterface, "LightDir", TW_TYPE_DIR3F, &lightDirection, " label='Light direction' opened=true group = 'DirectionalLight' ");
	TwAddVarRW(mUserInterface, "Dir Light Ambient", TW_TYPE_COLOR3F, &dirLightAColor, " label=' Dir Light 1 Ambient' group = 'DirectionalLight' ");
	TwAddVarRW(mUserInterface, "Dir Light Diffuse", TW_TYPE_COLOR3F, &dirLightDColor, " label=' Dir Light 1 Diffuse' group = 'DirectionalLight' ");
	TwAddVarRW(mUserInterface, "Dir Light Specular", TW_TYPE_COLOR3F, &dirLightSColor, " label=' Dir Light 1 Specular' group = 'DirectionalLight' ");
	TwAddVarRW(mUserInterface, "isActive DirLight", TW_TYPE_BOOLCPP, &isActiveDirLight, " group = 'DirectionalLight' ");

	//SPOT LIGHT
	TwAddVarRW(mUserInterface, "OuterCutoff ", TW_TYPE_FLOAT, &outerCutOff, " min=0 max=90 step=0.01 label=' OuterCutoff' group = 'SpotLight' ");
	TwAddVarRW(mUserInterface, "InnerCutOff ", TW_TYPE_FLOAT, &innerCutOff, " min=0 max=90 step=0.01 label=' InnerCutOff' group = 'SpotLight' ");
	TwAddVarRW(mUserInterface, "SpotLight Ambient", TW_TYPE_COLOR3F, &spotLightAColor, " label=' SpotLight Ambient' group = 'SpotLight' ");
	TwAddVarRW(mUserInterface, "SpotLight Diffuse", TW_TYPE_COLOR3F, &spotLightDColor, " label=' SpotLight Diffuse' group = 'SpotLight' ");
	TwAddVarRW(mUserInterface, "SpotLight Specular", TW_TYPE_COLOR3F, &spotLightSColor, " label=' SpotLight Specular' group = 'SpotLight' ");
	TwAddVarRW(mUserInterface, "ConstantAtt Spotlight", TW_TYPE_FLOAT, &spotLightAttConsColor, " min=0 max=64 step=0.01 label=' ConstantAtt' group = 'SpotLight' ");
	TwAddVarRW(mUserInterface, "LinearAtt Spotlight", TW_TYPE_FLOAT, &spotLightAttLiColor, " min=0 max=64 step=0.01 label=' LinearAtt' group = 'SpotLight' ");
	TwAddVarRW(mUserInterface, "QuadraticAtt Spotlight", TW_TYPE_FLOAT, &spotLightAttCuaColor, " min=0 max=64 step=0.01 label=' QuadraticAtt' group = 'SpotLight' ");
	TwAddVarRW(mUserInterface, "isActive SpotLight", TW_TYPE_BOOLCPP, &isActiveSpotLight, " group = 'SpotLight' ");


	//MATERIAL 
	TwAddVarRW(mUserInterface, "Shininess", TW_TYPE_FLOAT, &shininess, " min=0 max=64 step=0.1 label=' Shininess' group = 'Blinn-Phong' ");
	TwAddVarRW(mUserInterface, "Roughness", TW_TYPE_FLOAT, &roughness, " min=0.01 max=64 step=0.01 label=' Roughness' group = 'Oren-Nayar/Cook-Torrance' ");
	TwAddVarRW(mUserInterface, "Reflectance", TW_TYPE_FLOAT, &reflectance, " min=0 max=64 step=0.01 label=' Reflectance' group = 'Cook-Torrance' ");
	TwAddVarRW(mUserInterface, "Intensity", TW_TYPE_FLOAT, &intensity, " min=0 max=64 step=0.01 label=' Intensity' group = 'Oren-Nayar/Cook-Torrance' ");


}

CUserInterface::~CUserInterface()
{
}

void CUserInterface::reshape(int width, int height)
{
	TwWindowSize(width, height);
}

void CUserInterface::show()
{
	TwDefine("Figure visible = true");
}

void CUserInterface::hide()
{
	TwDefine("Figure visible = false");
}


float* CUserInterface::getModelAmbientColor()
{
	return modelAColor;
}

float* CUserInterface::getModelDiffuseColor()
{
	return modelDColor;
}

float* CUserInterface::getModelSpecularColor()
{
	return modelSColor;
}


float *CUserInterface::getPointLight1AmbientColor() {
	return pointLight1AColor;
}


float *CUserInterface::getPointLight1DiffuseColor() {
	return pointLight1DColor;
}

float *CUserInterface::getPointLight1SpecularColor() {
	return pointLight1SColor;
}

float CUserInterface::getPointLight1AttConsColor() {
	return pointLight1AttConsColor;
}

float CUserInterface::getPointLight1AttLiColor() {
	return pointLight1AttLiColor;
}

float CUserInterface::getPointLight1AttCuaColor() {
	return pointLight1AttCuaColor;
}


float *CUserInterface::getPointLight2AmbientColor() {
	return pointLight2AColor;
}

float *CUserInterface::getPointLight2DiffuseColor() {
	return pointLight2DColor;
}


float *CUserInterface::getPointLight2SpecularColor() {
	return pointLight2SColor;
}

float CUserInterface::getPointLight2AttConsColor() {
	return pointLight2AttConsColor;
}

float CUserInterface::getPointLight2AttLiColor() {
	return pointLight2AttLiColor;
}

float CUserInterface::getPointLight2AttCuaColor() {
	return pointLight2AttCuaColor;
}


float *CUserInterface::getDirLightAmbientColor() {
	return dirLightAColor;
}

float *CUserInterface::getDirLightDiffuseColor() {
	return dirLightDColor;
}

float *CUserInterface::getDirLightSpecularColor() {
	return dirLightSColor;
}

glm::vec3 CUserInterface::getPointLight1Translation() {
	return pointLight1Translation;
}

void CUserInterface::setPointLight1Translation( glm::vec3 pos ) {
	pointLight1Translation = pos;
}

glm::vec3 CUserInterface::getPointLight2Translation() {
	return pointLight2Translation;
}

void CUserInterface::setPointLight2Translation(glm::vec3 pos) {
	pointLight2Translation = pos;
}

bool  CUserInterface::getEnablePointLight1() {
	return g_enablePointLight1;
}

bool  CUserInterface::getEnablePointLight2() {
	return g_enablePointLight2;
}

float CUserInterface::getShininess() {
	return shininess;
}

float CUserInterface::getOuterCutOff() {
	return outerCutOff;
}


float CUserInterface::getInnerCutOff() {
	return innerCutOff;
}


float CUserInterface::getSpotLightAttConsColor() {
	return spotLightAttConsColor;
}


float CUserInterface::getSpotLightAttLiColor() {
	return spotLightAttLiColor;
}

float CUserInterface::getSpotLightAttCuaColor() {
	return spotLightAttCuaColor;
}


float *CUserInterface::getSpotLightAmbientColor() {
	return spotLightAColor;
}

float *CUserInterface::getSpotLightDiffuseColor() {
	return spotLightDColor;
}

float *CUserInterface::getSpotLightSpecularColor() {
	return spotLightSColor;
}

bool CUserInterface::getIsActivePointLight1() {
	return isActivePointLight1;
}

bool CUserInterface::getIsActivePointLight2() {
	return isActivePointLight2;
}

bool CUserInterface::getIsActiveDirLight() {
	return isActiveDirLight;
}

bool CUserInterface::getIsActiveSpotLight() {
	return isActiveSpotLight;
}

float CUserInterface::getRoughness() {
	return roughness;
}

float CUserInterface::getIntensity() {
	return intensity;
}

float CUserInterface::getReflectance() {
	return reflectance;
}

