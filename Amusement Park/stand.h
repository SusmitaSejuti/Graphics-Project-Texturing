#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "drawcube.h"
void stand(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 als)
{
    float rotateAngle_Y = 0.0;


    glm::mat4 rotateY = glm::mat4(1.0f);
    glm::mat4 rotateZ = glm::mat4(1.0f);
    float rotateAngle_Z = 0.0f;
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    
    for (float i = 0.0f; i < 360.0f; i += 2.50f) {
        //Left stand

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.250f, 3.0f, -2.15));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z + i), glm::vec3(0.0f, 1.0f, 0.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.055, -4.095, 0.005));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 1, 1));
        model = als * translateMatrix * rotateYMatrix * scaleMatrix;

        drawCube(cubeVAO, lightingShader, model, 0.9, 0.9, 0.9); //this model is later sent to the vertex shader. //altogether is a matrix that is multiplied with every parts of the bed, so everything is part of Altogether. when rotated, everything is rotated AllTogether
        //glBindVertexArray(VAO);
    }
    
    //for (float i = 0.0f; i < 360.0f; i += 2.50f) {
    //    //Right stand

    //    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.50f, 2.95f, -2.10f));
    //    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z + i), glm::vec3(0.0f, 1.0f, 0.0f));
    //    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.055, -4.095, 0.005));
    //    model = als * translateMatrix * rotateYMatrix * scaleMatrix;

    //    drawCube(cubeVAO, lightingShader, model, 0.9, 0.9, 0.9); //this model is later sent to the vertex shader. //altogether is a matrix that is multiplied with every parts of the bed, so everything is part of Altogether. when rotated, everything is rotated AllTogether
    //    //glBindVertexArray(VAO);
    //}
    //
    //for (float i = 89.9f; i<= 90.0f; i += 2.5f) {
    //    //Center stand

    //    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.75f, 2.95f, -2.1f));
    //    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z + i), glm::vec3(.5f, -0.5f, 5.0f));
    //    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.055, -4.095, 0.005));
    //    model = als * translateMatrix * rotateYMatrix * scaleMatrix;

    //    drawCube(cubeVAO, lightingShader, model, 0.9, 0.9, 0.9); //this model is later sent to the vertex shader. //altogether is a matrix that is multiplied with every parts of the bed, so everything is part of Altogether. when rotated, everything is rotated AllTogether
    //    //glBindVertexArray(VAO);
    //}
    
}
#pragma once
#pragma once
