#pragma once

#include <AntTweakBar.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

using std::string;

//Singleton user interface class

class CUserInterface
{
private:
	static CUserInterface * mInterface; //Holds the instance of the class
	TwBar *mUserInterface;
	glm::vec3 pointLight1Translation;
	glm::vec3 pointLight2Translation;

	bool g_enablePointLight1;
	bool g_enablePointLight2;

	float modelAColor[3] = { .5f, 0, 0};
	float modelDColor[3] = { 1, 1, 0};
	float modelSColor[3] = { .5f, .5f, .5f};


	float pointLight1AColor[3] = { 0, 0, 0};
	float pointLight1DColor[3] = { 0, .5, 0};
	float pointLight1SColor[3] = { 1, 1, 1};
	float pointLight1AttConsColor = .5f;
	float pointLight1AttLiColor = .1f;
	float pointLight1AttCuaColor = 0.01f;

	bool isActivePointLight1 = true;

	float pointLight2AColor[3] = { 0, 0, 0};
	float pointLight2DColor[3] = { .5, 0, 0};
	float pointLight2SColor[3] = { 1, 1, 1};
	float pointLight2AttConsColor = .5f;
	float pointLight2AttLiColor = .1f;
	float pointLight2AttCuaColor = 0.01f;

	bool isActivePointLight2 = true;

	float dirLightAColor[3] = { 0, 0, 0 };
	float dirLightDColor[3] = { .5, .3f, 0 };
	float dirLightSColor[3] = { .5, .4f, 0 };

	bool isActiveDirLight = true;

	float outerCutOff = .89f;
	float innerCutOff = 0.93f;
	float spotLightAColor[3] = { 0,0,0 };
	float spotLightDColor[3] = { 1,1,1 };
	float spotLightSColor[3] = { 1,1,1 };
	float spotLightAttConsColor = .5f;
	float spotLightAttLiColor = .1f;
	float spotLightAttCuaColor = 0.01f;

	bool isActiveSpotLight = true;

	
	float shininess = 32;
	float roughness = .3f;
	float intensity = 1;
	float reflectance = 0.5;

	float lightDirection[3] = { 0.72f, -0.69f, 0.0f };


public:
	///Method to obtain the only instance of the calls
	static CUserInterface * Instance();
	~CUserInterface();
	void reshape(int width, int height);
	void show();
	void hide();

	float *getModelAmbientColor();
	float *getModelDiffuseColor();
	float *getModelSpecularColor();

	float *getPointLight1AmbientColor();
	float *getPointLight1DiffuseColor();
	float *getPointLight1SpecularColor();
	float getPointLight1AttConsColor();
	float getPointLight1AttLiColor();
	float getPointLight1AttCuaColor();

	float *getPointLight2AmbientColor();
	float *getPointLight2DiffuseColor();
	float *getPointLight2SpecularColor();
	float getPointLight2AttConsColor();
	float getPointLight2AttLiColor();
	float getPointLight2AttCuaColor();

	float *getDirLightAmbientColor();
	float *getDirLightDiffuseColor();
	float *getDirLightSpecularColor();

	glm::vec3 getPointLight1Translation();
	void setPointLight1Translation( glm::vec3 pos);
	glm::vec3 getPointLight2Translation();
	void setPointLight2Translation( glm::vec3 pos);

	bool getEnablePointLight1();
	bool getEnablePointLight2();

	float getShininess();
	float getRoughness();
	float getIntensity();
	float getReflectance();

	float* getLightDirection() {
		return lightDirection;
	}


	float getOuterCutOff();
	float getInnerCutOff();

	float *getSpotLightAmbientColor();
	float *getSpotLightDiffuseColor();
	float *getSpotLightSpecularColor();

	float getSpotLightAttConsColor();
	float getSpotLightAttLiColor();
	float getSpotLightAttCuaColor();


	bool getIsActivePointLight1();
	bool getIsActivePointLight2();

	bool getIsActiveDirLight();

	bool getIsActiveSpotLight();

private:
	///Private constructor
	CUserInterface();
};