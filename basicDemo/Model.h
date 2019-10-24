#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

using namespace std;

enum MaterialType {
	blinnPhong,
	orenNayar,
	cookTorrance
};


class Model{

private:

	vector < glm::vec3 > out_vertices;
	vector < glm::vec2 > out_uvs;
	vector < glm::vec3 > out_normals;
	glm::vec3 position;
	MaterialType material;
	unsigned int textureID;
	bool hasTexture;

	// Index (GPU) of the geometry buffer
	unsigned int VBO;
	// Index (GPU) vertex array object
	unsigned int VAO;

	GLuint colorBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;

public:

	Model();

	~Model();


	bool LoadObj(const char * path);


	void BuildGeometry();

	unsigned int GetVAO();

	unsigned int GetVBO();

	GLuint GetColorBuffer();

	GLuint GetUvBuffer();

	GLuint GetNormalBuffer();

	int GetNumTriangles();

	glm::vec3 getPosition();

	void setPosition(glm::vec3 pos);

	void setMaterial(MaterialType type);

	MaterialType getMaterial();

	void setTextureID(unsigned int _textureID);
	unsigned int getTextureID();

};