#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "drawcube.h"

void bench(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 als)
{
    //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    //glm::mat4 translateMatrix5, rotateXMatrix5, rotateYMatrix5, rotateZMatrix5, scaleMatrix5, model5;
    //glm::mat4 translateMatrix6, rotateXMatrix6, rotateYMatrix6, rotateZMatrix6, scaleMatrix6, model6;


    //translateMatrix5 = glm::translate(identityMatrix, glm::vec3(-0.1, -0.82, -0.2));
    //rotateXMatrix5 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //rotateYMatrix5 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //rotateZMatrix5 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //scaleMatrix5 = glm::scale(identityMatrix, glm::vec3(2.0, 1.0, 0.05));
    //model5 = translateMatrix5 * rotateXMatrix5 * rotateYMatrix5 * rotateZMatrix5 * scaleMatrix5;
    //drawCube(cubeVAO, lightingShader, model5, 0.6, 0.6f, 0.125f);

    //// Seat
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.05, 0.15, -0.875));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, .15f, 0.5f));
    //model = als * translateMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model, 0.886, 0.529f, 0.263f);

    ////Back
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.05, 0.3, -0.8));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, .5f, 0.1f));
    //model = als * translateMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model, 0.392, 0.188f, 0.035f);

    //// leg1
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.05, 0.2, -0.8));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.025f, -.5f, 0.05f));
    //model = als * translateMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model, 0.05, 0.1f, 0.125f);

    //// leg2
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.05, 0.2, -0.5));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.025f, -.5f, 0.05f));
    //model = als * translateMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model, 0.05, 0.1f, 0.125f);

    //// leg3
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.85, 0.2, -0.9 ));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.025f, -.5f, 0.05f));
    //model = als * translateMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model, 0.05, 0.1f, 0.125f);
   
    //
    ////leg4
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.85 , 0.2,- 0.5));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.025f, -0.5f, 0.05f));
    //model = als * translateMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model, 0.05, 0.1f, 0.125f);
  
}

#pragma once
