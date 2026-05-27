#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


// A 3D thing to draw
class cMesh
{
public:
	cMesh(std::string theModelname);
	std::string modelName;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	float scale = 1.0f;
};