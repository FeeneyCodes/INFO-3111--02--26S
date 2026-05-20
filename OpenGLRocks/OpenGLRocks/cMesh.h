#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class cMesh
{
public:
	std::string modelName;
	glm::vec3 position;
	glm::vec3 rotation;
	float scale;
};