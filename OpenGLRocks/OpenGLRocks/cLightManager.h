#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "globalOpenGLStuff.h"

// This is to match the one in the shader
struct sLight
{
	glm::vec3 position = glm::vec3(0.0f);	
	glm::vec4 diffuseRGBA = glm::vec4(1.0f);
	// We likely won't use this ON THE LIGHT
	glm::vec3 specularHighlightColour = glm::vec3(1.0f);
	float specularPower = 1.0f;	
	//vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	float attenuationConstant = 0.1f;
	float attenuationLinear = 0.001f;
	float attenuationQuadratic = 0.0001f;
	float attenuationDistanceCutOff = FLT_MAX;
	
	glm::vec3 direction = glm::vec3(0.0f);	// Spot, directional lights
	//vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	// 0 = pointlight
	// 1 = spot light
	// 2 = directional light
	enum eLightType
	{
		POINT_LIGHT = 0,
		SPOT_LIGHT = 1,
		DIRECTION = 2
	};
	int lightType = POINT_LIGHT;
	float spotInnerAngle = 10.0;
	float spotOuterAngle = 20.0f;
	//vec4 param2;	// x = 0 for off, 1 for on
	bool bIsOn = false;

	// Here's the uniform locations for all the shader uniforms
	GLint position_UL = -1;		
	GLint diffuse_UL = -1;
	GLint specular_UL = -1;
	GLint atten_UL = -1;
	GLint direction_UL = -1;
	GLint param1_UL = -1;
	GLint param2_UL = -1;
};


class cLightManager
{
public:
	static const int NUMBER_OF_LIGHT = 8;
	sLight myLights[NUMBER_OF_LIGHT];
	
	// GLuint is an unsigned int
	// Called ONCE
	void SetUpUniformShaderLocations(unsigned int shdaerProgramID);
	// Called every frame
	void CopyLightInfoToShader(unsigned int shaderProgID);
};

