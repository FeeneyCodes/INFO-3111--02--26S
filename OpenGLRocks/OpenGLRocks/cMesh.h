#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <string>

// A 3D thing to draw
class cMesh
{
public:
	cMesh(std::string theModelname);
	std::string modelName;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	float scale = 1.0f;
	//
	glm::vec3 diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
	//glm::vec3 specularRGBP;
	bool bIsWireframe = false;
	// 
	float alphaTransparency = 1.0f;
	static const unsigned int NUM_TEXTURES = 8;
	std::string textures[NUM_TEXTURES];
	float textureBlendRatio[NUM_TEXTURES];
};