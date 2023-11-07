#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "drawcube.h"

void swing(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 als)
{
    float rotateAngle_Y = 0.0;


    glm::mat4 rotateY = glm::mat4(1.0f);
    glm::mat4 rotateZ = glm::mat4(1.0f);
    float rotateAngle_Z = 0.0f;
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //Swing
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.9, 1.0, -2.37));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, .15f, 0.5f));
    model = als * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.886, 0.529f, 0.263f);//226, 135, 67

    //swing leg1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.8, 1.0, -2.17));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.025f, 1.9f, 0.05f));
    model = als * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.392, 0.188f, 0.035f);//100, 48, 9, 1

    //Swing leg3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0, 1.0, -2.17));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.025f, 1.9f, 0.05f));
    model = als * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.392, 0.188f, 0.035f);


}
#pragma once
#pragma once