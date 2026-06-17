#include "cLightManager.h"

#include "globalOpenGLStuff.h"

void cLightManager::SetUpUniformShaderLocations(unsigned int shdaerProgramID)
{
	GLint eyeLocation_UL = glGetUniformLocation(shdaerProgramID, "eyeLocation");

	this->myLights[0].position_UL 
		= glGetUniformLocation(shdaerProgramID, "theLights[0].position");

	this->myLights[0].diffuse_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[0].diffuse");

	this->myLights[0].specular_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[0].specular");

	this->myLights[0].atten_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[0].atten");

	this->myLights[0].direction_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[0].direction");

	this->myLights[0].param1_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[0].param1");

	this->myLights[0].param2_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[0].param2");


	this->myLights[1].position_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[1].position");

	this->myLights[1].diffuse_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[1].diffuse");

	this->myLights[1].specular_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[1].specular");

	this->myLights[1].atten_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[1].atten");

	this->myLights[1].direction_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[1].direction");

	this->myLights[1].param1_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[1].param1");

	this->myLights[1].param2_UL
		= glGetUniformLocation(shdaerProgramID, "theLights[1].param2");

	// And so on for the remaining light...

}



void cLightManager::CopyLightInfoToShader(unsigned int shaderProgID)
{
	// Copy the C++ side structure from the Light array
	//	to the shader uniform variables


	// c++ 		glm::vec3 position = glm::vec3(0.0f);	
	// GLSL:	vec4 position;	// xyz, ignoring w (4th parameter)	

	//GLint position_UL = glGetUniformLocation(shaderProgID, "theLights[0].position");
	for (unsigned int lightIndex = 0; lightIndex < cLightManager::NUMBER_OF_LIGHT; lightIndex++)
	{
		glUniform4f( this->myLights[lightIndex].position_UL,
					 this->myLights[lightIndex].position.x,
					 this->myLights[lightIndex].position.y,
					 this->myLights[lightIndex].position.z,
					 1.0f);

		glUniform4f( this->myLights[lightIndex].diffuse_UL,
					 this->myLights[lightIndex].diffuseRGBA.r,
					 this->myLights[lightIndex].diffuseRGBA.g,
					 this->myLights[lightIndex].diffuseRGBA.b,
			         this->myLights[lightIndex].diffuseRGBA.a);

		//vec4 specular;	// rgb = highlight colour, w = power
		glUniform4f(this->myLights[lightIndex].specular_UL,
			this->myLights[lightIndex].specularHighlightColour.r,
			this->myLights[lightIndex].specularHighlightColour.g,
			this->myLights[lightIndex].specularHighlightColour.b,
			this->myLights[lightIndex].specularPower);

		// vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
		glUniform4f(this->myLights[lightIndex].atten_UL,
			this->myLights[lightIndex].attenuationConstant,
			this->myLights[lightIndex].attenuationLinear,
			this->myLights[lightIndex].attenuationQuadratic,
			this->myLights[lightIndex].attenuationDistanceCutOff);

		//vec4 direction;	// Spot, directional lights, ignoring w (4th parameter)	
		glUniform4f(this->myLights[lightIndex].direction_UL,
			this->myLights[lightIndex].direction.x,
			this->myLights[lightIndex].direction.y,
			this->myLights[lightIndex].direction.z,
			1.0f);

		// vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
		glUniform4f(this->myLights[lightIndex].param1_UL,
			(GLfloat)this->myLights[lightIndex].lightType,
			this->myLights[lightIndex].spotInnerAngle,
			this->myLights[lightIndex].spotOuterAngle,
			1.0f);

		// vec4 param2;	// x = 0 for off, 1 for on
		glUniform4f(this->myLights[lightIndex].param2_UL,
			(this->myLights[lightIndex].bIsOn ? 1.0f : 0.0f),
			1.0f, 1.0f, 1.0f);


	}//for (unsigned int lightIndex

	return;
}
