#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <stb_image.h>


#include "UserInterface.h"

#include "Shader.h"
#include "Model.h"
#include "Light.h"

const int NUM_POINTLIGHT = 2;

struct LightColor {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct Attenuation {
	float constant;
	float linear;
	float quadratic;
};

struct PointLightProperties {
	glm::vec3 position;
	LightColor color;
	Attenuation attenuation;
};


struct DirectionalLightProperties {
	glm::vec3 direction;
	LightColor color;
};

struct SpotLightProperties {
	glm::vec3 position;
	glm::vec3 direction;
	LightColor color;
	float cutOff;
	float outerCutOff;
	Attenuation attenuation;
};


struct Data {
	glm::vec3 vertexPos;
	glm::vec3 normal;
};

vector<Model *> modelsBlinnPhong;
vector<Model *> modelsOrenNayar;
vector<Model *> modelsCookTorrance;

Light *lightSources[2];
PointLightProperties pointLights[2];
DirectionalLightProperties directionalLight;
SpotLightProperties spotLight;
float shininess = 32;
float roughness = .3;
float intensity = 1;
float reflectance = 0.8;


// Window current width
unsigned int windowWidth = 800;
// Window current height
unsigned int windowHeight = 600;
// Window title
const char *windowTitle = "Tarea1";
// Window pointer
GLFWwindow *window;
// User Interface
CUserInterface * userInterface;
// Right button is currently pressed
bool rightButtonPressed = false;

// Shader for regular models
Shader *shaderBlinnPhong;
Shader *shaderOrenNayar;
Shader *shaderCookTorrance;
// Shader for lights
Shader *shaderLights;

// Index (GPU) of the texture
unsigned int houseTextureID;
unsigned int planeTextureID;

glm::vec3 position = glm::vec3( 0, 0, 5);
// horizontal angle : toward -Z
float horizontalAngle = -3.14;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

glm::vec3 direction;
glm::vec3 up;


//ACTIVE LIGHTS BOOLEANS
bool isActiveSpotLight = true;
bool isActiveDirLight = true;
bool isActivePointLight1 = true;
bool isActivePointLight2 = true;


//POSITION TESTING MODELS
glm::vec3 modelPosition1 = glm::vec3(0, 0, -30); //BLINN-PHONG
glm::vec3 modelPosition2 = glm::vec3(0, 0, 0); //OREN-NAYAR
glm::vec3 modelPosition3 = glm::vec3(0, 0, 30); //COOK-TORRANCE

glm::vec3 planePosition = glm::vec3(0, 0, 0);


/**
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void resize(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    // Sets the OpenGL viewport size and position
    glViewport(0, 0, windowWidth, windowHeight);
	userInterface->reshape(windowWidth,windowHeight);
}

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (TwEventMouseButtonGLFW(button, action))
		return;
}

void cursorPos(GLFWwindow* window, double x, double y)
{
	if (TwEventMousePosGLFW(int(x), int(y)))
		return;
}

void charInput(GLFWwindow* window, unsigned int scanChar)
{
	if (TwEventCharGLFW(scanChar, GLFW_PRESS))
		return;
}


/**
 * Initialize the glfw library
 * @returns{bool} true if everything goes ok
 * */
bool initWindow()
{
	// Initialize glfw
	glfwInit();
	// Sets the Opegl context to Opengl 3.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates the window
	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

	// The window couldn't be created
	if (!window)
	{
		std::cout << "Failed to create the glfw window" << std::endl;
		glfwTerminate(); // Stops the glfw program
		return false;
	}

	// Creates the glfwContext, this has to be made before calling initGlad()
	glfwMakeContextCurrent(window);

	// Window resize callback
	glfwSetFramebufferSizeCallback(window, resize);

	// Other callbacks
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetCursorPosCallback(window, cursorPos);
	glfwSetCharCallback(window, charInput);


    return true;
}
/**
 * Initialize the glad library
 * @returns{bool} true if everything goes ok
 * */
bool initGlad()
{
    // Initialize glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // If something went wrong during the glad initialization
    if (!status)
    {
        std::cout << status << std::endl;
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}


bool initUserInterface()
{

	if (!TwInit(TW_OPENGL_CORE, NULL))
		return false;


	TwWindowSize(windowWidth, windowHeight);
	userInterface = CUserInterface::Instance();

	return true;
}

void UpdateUserInterface() {

	//get blinn phong parameters
	shininess = userInterface->getShininess();
	//get oren nayar parameters
	roughness = userInterface->getRoughness();
	//get cook torrance parameters
	reflectance = userInterface->getReflectance();

	//oren-nayar cook-torrance
	intensity = userInterface->getIntensity();

	//POINT LIGHT 1
	pointLights[0].position = userInterface->getPointLight1Translation();
	
	float* ambientPointLight1 = userInterface->getPointLight1AmbientColor();
	pointLights[0].color.ambient = glm::vec3(ambientPointLight1[0], ambientPointLight1[1], ambientPointLight1[2]);

	float* diffusePointLight1 = userInterface->getPointLight1DiffuseColor();
	pointLights[0].color.diffuse = glm::vec3(diffusePointLight1[0], diffusePointLight1[1], diffusePointLight1[2]);

	float* specularPointLight1 = userInterface->getPointLight1SpecularColor();
	pointLights[0].color.specular = glm::vec3(specularPointLight1[0], specularPointLight1[1], specularPointLight1[2]);


	pointLights[0].attenuation.constant  = userInterface->getPointLight1AttConsColor();
	pointLights[0].attenuation.linear = userInterface->getPointLight1AttLiColor();
	pointLights[0].attenuation.quadratic = userInterface->getPointLight1AttCuaColor();

	isActivePointLight1 = userInterface->getIsActivePointLight1();

	//POINT LIGHT 2
	pointLights[1].position = userInterface->getPointLight2Translation();

	float* ambientPointLight2 = userInterface->getPointLight2AmbientColor();
	pointLights[1].color.ambient = glm::vec3(ambientPointLight2[0], ambientPointLight2[1], ambientPointLight2[2]);

	float* diffusePointLight2 = userInterface->getPointLight2DiffuseColor();
	pointLights[1].color.diffuse = glm::vec3(diffusePointLight2[0], diffusePointLight2[1], diffusePointLight2[2]);

	float* specularPointLight2 = userInterface->getPointLight2SpecularColor();
	pointLights[1].color.specular = glm::vec3(specularPointLight2[0], specularPointLight2[1], specularPointLight2[2]);


	pointLights[1].attenuation.constant = userInterface->getPointLight2AttConsColor();
	pointLights[1].attenuation.linear = userInterface->getPointLight2AttLiColor();
	pointLights[1].attenuation.quadratic = userInterface->getPointLight2AttCuaColor();

	isActivePointLight2 = userInterface->getIsActivePointLight2();

	//DIRECTIONAL LIGHT (SUN)
	float* lightDirection = userInterface->getLightDirection();
	directionalLight.direction = glm::vec3(lightDirection[0], lightDirection[1], lightDirection[2]);

	float* ambientDirLight = userInterface->getDirLightAmbientColor();
	directionalLight.color.ambient = glm::vec3(ambientDirLight[0], ambientDirLight[1], ambientDirLight[2]);

	float* diffuseDirLight = userInterface->getDirLightDiffuseColor();
	directionalLight.color.diffuse = glm::vec3(diffuseDirLight[0], diffuseDirLight[1], diffuseDirLight[2]);

	float* specularDirLight = userInterface->getDirLightSpecularColor();
	directionalLight.color.specular = glm::vec3(specularDirLight[0], specularDirLight[1], specularDirLight[2]);

	isActiveDirLight = userInterface->getIsActiveDirLight();

	//SPOT LIGHT
	spotLight.outerCutOff = userInterface->getOuterCutOff();
	spotLight.cutOff = userInterface->getInnerCutOff();

	spotLight.attenuation.constant = userInterface->getSpotLightAttConsColor();
	spotLight.attenuation.linear = userInterface->getSpotLightAttLiColor();
	spotLight.attenuation.quadratic = userInterface->getSpotLightAttCuaColor();

	float* ambientSpotLight = userInterface->getSpotLightAmbientColor();
	spotLight.color.ambient = glm::vec3(ambientSpotLight[0], ambientSpotLight[1], ambientSpotLight[2]);

	float* diffuseSpotLight = userInterface->getSpotLightDiffuseColor();
	spotLight.color.diffuse = glm::vec3(diffuseSpotLight[0], diffuseSpotLight[1], diffuseSpotLight[2]);

	float* specularSpotLight = userInterface->getSpotLightSpecularColor();
	spotLight.color.specular = glm::vec3(specularSpotLight[0], specularSpotLight[1], specularSpotLight[2]);

	isActiveSpotLight = userInterface->getIsActiveSpotLight();
}


/**
 * Initialize the opengl context
 * */
void initGL()
{
    // Enables the z-buffer test
    glEnable(GL_DEPTH_TEST);
    // Sets the ViewPort
    glViewport(0, 0, windowWidth, windowHeight);
    // Sets the clear color
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

}

/**
 * Loads a texture into the GPU
 * @param{const char} path of the texture file
 * @returns{unsigned int} GPU texture index
 * */
unsigned int loadTexture(const char *path)
{
    unsigned int id;
    // Creates the texture on GPU
    glGenTextures(1, &id);
    // Loads the texture
    int textureWidth, textureHeight, numberOfChannels;
    // Flips the texture when loads it because in opengl the texture coordinates are flipped
    stbi_set_flip_vertically_on_load(true);
    // Loads the texture file data
    unsigned char *data = stbi_load(path, &textureWidth, &textureHeight, &numberOfChannels, 0);
    if (data)
    {
        // Gets the texture channel format
        GLenum format;
        switch (numberOfChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }

        // Binds the texture
        glBindTexture(GL_TEXTURE_2D, id);
        // Creates the texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        // Creates the texture mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set the filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "ERROR:: Unable to load texture " << path << std::endl;
        glDeleteTextures(1, &id);
    }
    // We dont need the data texture anymore because is loaded on the GPU
    stbi_image_free(data);

    return id;
}
/**
 * Initialize everything
 * @returns{bool} true if everything goes ok
 * */
bool init()
{
    // Initialize the window, and the glad components
    if (!initWindow() || !initGlad() || !initUserInterface() )
        return false;

    // Initialize the opengl context
    initGL();

    // Loads the shader
	shaderLights = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	shaderBlinnPhong = new Shader("assets/shaders/lightningBlingPhong.vert", "assets/shaders/lightningBlingPhong.frag");
	shaderOrenNayar = new Shader("assets/shaders/lightningOrenNayar.vert", "assets/shaders/lightningOrenNayar.frag");
	shaderCookTorrance = new Shader("assets/shaders/lightningCookTorrance.vert", "assets/shaders/lightningCookTorrance.frag");

	#pragma region loadTextures

	// Loads the texture into the GPU
	houseTextureID = loadTexture("assets/textures/cottage_diffuse.png");

	planeTextureID = loadTexture("assets/textures/plane_diffuse.jpg");

	#pragma endregion



	#pragma region LoadModels

	string pathCube = "./assets/models/cube.obj";
	string pathHouse = "./assets/models/cottage/cottage.obj";
	string pathPlane = "./assets/models/plane.obj";


	Model *model1 = new Model();
	if (model1->LoadObj(pathHouse.c_str())) {
		model1->BuildGeometry();

		modelsBlinnPhong.push_back(model1);
	}

	model1->setPosition(modelPosition1);
	model1->setMaterial(blinnPhong);
	model1->setTextureID(houseTextureID);

	Model *model2 = new Model();
	if (model2->LoadObj(pathHouse.c_str())) {
		model2->BuildGeometry();

		modelsOrenNayar.push_back(model2);
	}

	model2->setPosition(modelPosition2);
	model2->setMaterial(orenNayar);
	model2->setTextureID(houseTextureID);

	Model *model3 = new Model();
	if (model3->LoadObj(pathHouse.c_str())) {
		model3->BuildGeometry();

		modelsCookTorrance.push_back(model3);
	}

	model3->setPosition(modelPosition3);
	model3->setMaterial(cookTorrance);
	model3->setTextureID(houseTextureID);

	Model *model4 = new Model();
	if (model4->LoadObj(pathPlane.c_str())) {
		model4->BuildGeometry();

		modelsBlinnPhong.push_back(model4);
	}

	model4->setPosition(planePosition);
	model4->setMaterial(blinnPhong);
	model4->setTextureID(planeTextureID);

	for (int i = 0; i < 2; i++) {

		lightSources[i] = new Light();
		if (lightSources[i]->LoadObj(pathCube.c_str())) {
			lightSources[i]->BuildGeometry();
		}
	}

	userInterface->setPointLight1Translation(glm::vec3(-21, 5, -30));
	userInterface->setPointLight2Translation(glm::vec3(-21, 5, -2));

	#pragma endregion

    return true;
}
/**
 * Process the keyboard input
 * There are ways of implementing this function through callbacks provide by
 * the GLFW API, check the GLFW documentation to find more
 * @param{GLFWwindow} window pointer
 * */
void processKeyboardInput(GLFWwindow *window)
{
    // Checks if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // Tells glfw to close the window as soon as possible
        glfwSetWindowShouldClose(window, true);

    // Checks if the r key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // Reloads the shader
        delete shaderBlinnPhong;
		delete shaderOrenNayar;
		delete shaderCookTorrance;

		shaderBlinnPhong = new Shader("assets/shaders/lightningBlingPhong.vert", "assets/shaders/lightningBlingPhong.frag");
		shaderOrenNayar = new Shader("assets/shaders/lightningOrenNayar.vert", "assets/shaders/lightningOrenNayar.frag");
		shaderCookTorrance = new Shader("assets/shaders/lightningCookTorrance.vert", "assets/shaders/lightningCookTorrance.frag");
    }

	// Check is the right click of the mouse is pressed
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		rightButtonPressed = true;
	}
	else {
		rightButtonPressed = false;
	}

	float deltaTime = 0.016;
	double currentTime = glfwGetTime();
	
	if (rightButtonPressed) {
		
		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);



		// Compute new orientation
		horizontalAngle += mouseSpeed /* * 1 */ * float(windowWidth / 2 - xpos);
		verticalAngle += mouseSpeed /* * deltaTime */ * float(windowHeight / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * deltaTime * speed * 5.0f;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * deltaTime * speed * 5.0f;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * deltaTime * speed * 5.0f;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * deltaTime * speed * 5.0f;
		}

	}
}


void RenderModelsMaterial(vector<Model *> materialModels, Shader *shaderMaterial, glm::mat4 modelMatrix
	, glm::mat4 view, glm::mat4 projection, glm::mat3 normalMatrix, MaterialType materialType) {

	

	shaderMaterial->use();

	shaderMaterial->setMat4("view", view);
	shaderMaterial->setMat4("proj", projection);

	//dirLight
	shaderMaterial->setVec3("dirLight.direction", directionalLight.direction);
	shaderMaterial->setVec3("dirLight.color.ambient", directionalLight.color.ambient);
	shaderMaterial->setVec3("dirLight.color.diffuse", directionalLight.color.diffuse);
	shaderMaterial->setVec3("dirLight.color.specular", directionalLight.color.specular);
	shaderMaterial->setBool("isActiveDirLight", isActiveDirLight);

	//spotLight

	shaderMaterial->setVec3("spotLight.position", position);
	shaderMaterial->setVec3("spotLight.direction", direction);
	shaderMaterial->setVec3("spotLight.color.ambient", spotLight.color.ambient);
	shaderMaterial->setVec3("spotLight.color.diffuse", spotLight.color.diffuse);
	shaderMaterial->setVec3("spotLight.color.specular", spotLight.color.specular);
	shaderMaterial->setFloat("spotLight.cutOff", spotLight.cutOff);
	shaderMaterial->setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
	shaderMaterial->setFloat("spotLight.attenuation.constant", spotLight.attenuation.constant);
	shaderMaterial->setFloat("spotLight.attenuation.linear", spotLight.attenuation.linear);
	shaderMaterial->setFloat("spotLight.attenuation.quadratic", spotLight.attenuation.quadratic);
	shaderMaterial->setBool("isActiveSpotLight", isActiveSpotLight);


	//pointLights
	for (int i = 0; i < 2; i++) {

		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".position", pointLights[i].position);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.ambient", pointLights[i].color.ambient);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.diffuse", pointLights[i].color.diffuse);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.specular", pointLights[i].color.specular);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.constant", pointLights[i].attenuation.constant);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.linear", pointLights[i].attenuation.linear);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.quadratic", pointLights[i].attenuation.quadratic);
	}

	shaderMaterial->setBool("isActivePointLight1", isActivePointLight1);
	shaderMaterial->setBool("isActivePointLight2", isActivePointLight2);



	if (materialType == blinnPhong) {
		//BLINN PHONG PARAMETERS
		shaderMaterial->setFloat("shininess", shininess);
	}else if (materialType == orenNayar) {
		//OREN NAYAR PARAMETERS
		shaderMaterial->setFloat("roughness", roughness);
		shaderMaterial->setFloat("intensity", intensity);
	}
	else if (materialType == cookTorrance) {
		//COOK TORRANCE PARAMETERS
		shaderMaterial->setFloat("roughness", roughness);
		shaderMaterial->setFloat("intensity", intensity);
		shaderMaterial->setFloat("reflectance", reflectance);
	}
	
	//GENERAL PARAMETERS
	shaderMaterial->setVec3("viewPos", position);

	//DRAW THE MODELS
	for (int i = 0; i < materialModels.size(); i++) {

		glBindTexture(GL_TEXTURE_2D, materialModels[i]->getTextureID() );

		modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = materialModels[i]->getPosition();
		modelMatrix = glm::translate(modelMatrix, modelPosition);

		shaderMaterial->setMat4("model", modelMatrix);

		// Binds the vertex array to be drawn
		glBindVertexArray(materialModels[i]->GetVAO());
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, materialModels[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}
}

/**
 * Render Function
 * */
void render()
{

    // Clears the color and depth buffers from the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);

	glm::mat4 view = glm::lookAt(
		position, // Camera is at (4,3,3), in world space
		position + direction, // and looks at the origin
		up  // Head is up (set to 0,-1,0 to look upside-down) 
	);

	//model matrix: an identity matrix (model will be at the origin)
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glm::mat3 normalMatrix = glm::mat3(1.0f);

	RenderModelsMaterial(modelsBlinnPhong, shaderBlinnPhong, modelMatrix, view, projection, normalMatrix, blinnPhong);

	RenderModelsMaterial(modelsOrenNayar, shaderOrenNayar, modelMatrix, view, projection, normalMatrix, orenNayar);

	RenderModelsMaterial(modelsCookTorrance, shaderCookTorrance, modelMatrix, view, projection, normalMatrix, cookTorrance);

	
	//DRAW THE LIGHTNINGS

	shaderLights->use();


	for (int i = 0; i < 2; i++) {

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, pointLights[i].position);
		glm::mat4 mvp = projection * view * modelMatrix; //Remember, matrix multimplication is the other way around

		shaderLights->setMat4("MVP", mvp);
		shaderLights->setVec3("colorIn", glm::vec3(0, 0, 1));

		glBindVertexArray(lightSources[i]->GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, lightSources[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}

	TwDraw();

	// Swap the buffer
	glfwSwapBuffers(window);

}
/**
 * App main loop
 * */
void update()
{
    // Loop until something tells the window, that it has to be closed
    while (!glfwWindowShouldClose(window))
    {
        // Checks for keyboard inputs
        processKeyboardInput(window);


        // Renders everything
        render();

		//Get the data from the user interface
		UpdateUserInterface();
		
        // Check and call events
        glfwPollEvents();
    }
}
/**
 * App starting point
 * @param{int} number of arguments
 * @param{char const *[]} running arguments
 * @returns{int} app exit code
 * */
int main(int argc, char const *argv[])
{
	/*Initialize variables*/

	//directional light
	directionalLight.direction = glm::vec3(0, 1, 1);
	directionalLight.color.ambient = glm::vec3(0, .3f, 0);
	directionalLight.color.diffuse = glm::vec3(.4f, 0, 0);
	directionalLight.color.specular = glm::vec3(0, .4f, .4f);


	for (int i = 0; i < NUM_POINTLIGHT; i++) {
		pointLights[i].position = glm::vec3(0, 0, 0);
		pointLights[i].attenuation.constant = .2f;
		pointLights[i].attenuation.linear = .2f;
		pointLights[i].attenuation.quadratic = 0.0f;
		pointLights[i].color.ambient = glm::vec3(.5, 0, 0);
		pointLights[i].color.diffuse = glm::vec3(0, 0.5, 0);
		pointLights[i].color.specular = glm::vec3(0, 0, .5);
	}

	spotLight.color.ambient = glm::vec3(1, 1, 1);
	spotLight.color.diffuse = glm::vec3(1, 1, 1);
	spotLight.color.specular = glm::vec3(1, 1, 1);
	spotLight.cutOff = .90f;
	spotLight.outerCutOff = .80f;
	spotLight.attenuation.constant = .2f;
	spotLight.attenuation.linear = .2f;
	spotLight.attenuation.quadratic = 0.0f;



    // Initialize all the app components
    if (!init())
    {
        // Something went wrong
        std::cin.ignore();
        return -1;
    }

    std::cout << "=====================================================" << std::endl
              << "        Press Escape to close the program            " << std::endl
              << "=====================================================" << std::endl;

    // Starts the app main loop
    update();

    // Deletes the texture from the gpu
    glDeleteTextures(1, &houseTextureID);
	glDeleteTextures(1, &planeTextureID);

	for (auto x : modelsBlinnPhong) {

		// Index (GPU) of the geometry buffer
		unsigned int VBO = x->GetVBO();
		// Index (GPU) vertex array object
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO );
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);

	}

	for (auto x : modelsOrenNayar) {

		// Index (GPU) of the geometry buffer
		unsigned int VBO = x->GetVBO();
		// Index (GPU) vertex array object
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);

	}


	for (auto x : modelsCookTorrance) {

		// Index (GPU) of the geometry buffer
		unsigned int VBO = x->GetVBO();
		// Index (GPU) vertex array object
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);

	}


    // Destroy the shader
    delete shaderBlinnPhong;
	delete shaderOrenNayar;
	delete shaderCookTorrance;
	// Destroy the shader lights
	delete shaderLights;

    // Stops the glfw program
    glfwTerminate();

    return 0;
}
