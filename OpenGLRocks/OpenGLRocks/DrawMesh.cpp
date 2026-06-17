#include "globalOpenGLStuff.h"

//#include "linmath.h"      // Another math library we aren't using
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cMesh.h"
#include "cVAOManager/cVAOManager.h"

extern cVAOManager* g_pVAOManager;

extern std::vector< cMesh* > g_vec_pModelsToDraw;


void DrawMesh( cMesh* pCurrentMesh, 
               GLuint program)
{
    // Is this visible 
    if ( ! pCurrentMesh->bIsVisible )
    {
        return;
    }

    glm::mat4 matModel = glm::mat4(1.0f);        // m          //mat4x4 m, p, mvp;

    // Movement or placement 
    glm::mat4 matTranslate
        = glm::translate(glm::mat4(1.0f),
            pCurrentMesh->position);

    // Rotation
    glm::mat4 matRotateX
        = glm::rotate(glm::mat4(1.0f),
            pCurrentMesh->rotation.x,
            glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 matRotateY
        = glm::rotate(glm::mat4(1.0f),
            pCurrentMesh->rotation.y,
            glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 matRotateZ
        = glm::rotate(glm::mat4(1.0f),
            pCurrentMesh->rotation.z,
            glm::vec3(0.0f, 0.0f, 1.0f));

    // Scaling
    glm::mat4 matScaleXYZ
        = glm::scale(glm::mat4(1.0f),
            glm::vec3(pCurrentMesh->scale,
                pCurrentMesh->scale,
                pCurrentMesh->scale));


    // Apply the scale matrix
    matModel = matScaleXYZ * matModel;  // Last applied
    // Apply the rotation matrices
    matModel = matRotateX * matModel;
    matModel = matRotateY * matModel;
    matModel = matRotateZ * matModel;
    // Apply the translation matrix
    matModel = matTranslate * matModel; // 1st applied

    // Copy the matModel to the shader, too
    // uniform mat4 mModel;
    GLint mModel_UL = glGetUniformLocation(program, "mModel");

    glUniformMatrix4fv(mModel_UL, 1, GL_FALSE,
        (const GLfloat*)&matModel);



    GLint colourRGB_UL = glGetUniformLocation(program, "colourRGB");

    glUniform3f(colourRGB_UL,
        pCurrentMesh->diffuseRGB.r,
        pCurrentMesh->diffuseRGB.g,
        pCurrentMesh->diffuseRGB.b);

    // Copy specular also
    //  uniform vec3 specularRGB;		// Colour of the highlight
    //  uniform float specularPower;	// How "shiny" it is (1.0 to	0)

    GLint specularRGB_UL = glGetUniformLocation(program, "specularRGB");
    GLint specularPower_UL = glGetUniformLocation(program, "specularPower");

    glUniform3f(specularRGB_UL,
        pCurrentMesh->specularHighlightColourRGB.r,
        pCurrentMesh->specularHighlightColourRGB.g,
        pCurrentMesh->specularHighlightColourRGB.b);

    glUniform1f(specularPower_UL, pCurrentMesh->specularPower);


    if (pCurrentMesh->bIsWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    std::string modelToDraw = pCurrentMesh->modelName;

    sModelDrawInfo theModelToDraw;
    if (::g_pVAOManager->FindDrawInfoByModelName(modelToDraw,
        theModelToDraw))
    {
        glBindVertexArray(theModelToDraw.VAO_ID);

        // Uses the index (element) buffer to look up
        //  into the bound vertex buffer and sends those final
        //  vertices to the vertex shader
        glDrawElements(GL_TRIANGLES,
            theModelToDraw.numberOfIndices, // How many indices
            GL_UNSIGNED_INT,
            (void*)0);      // What index to start from

        //disable VAO(and everything else)
        glBindVertexArray(0);
    }


    return;
}



cMesh* g_pFindMeshByFriendlyName(std::string theNameToFind)
{
    // 100
    for (std::vector< cMesh* >::iterator it_pMesh = ::g_vec_pModelsToDraw.begin();
        it_pMesh != ::g_vec_pModelsToDraw.end();
        it_pMesh++)
    {
        cMesh* pCurrentMesh = *it_pMesh;

        // This the one? 
        if (pCurrentMesh->friendlyName == theNameToFind)
        {
            // Yup
            return pCurrentMesh;
        }
    }
    // didn't find it
    return NULL;
}

