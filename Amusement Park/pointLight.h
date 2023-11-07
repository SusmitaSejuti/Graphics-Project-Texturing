//
//  pointLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef pointLight_h
#define pointLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class PointLight { //point light class
public:
    //the attributes of light
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    float k_c;
    float k_l;
    float k_q;
    int lightNumber; //which light

    //constructor
    PointLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num) {

        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        lightNumber = num;
    }
    //input is a shader, ekhan theke set up hochhe fragment shader e
    void setUpPointLight(Shader& lightingShader)
    {
        lightingShader.use();
        //setting up all the properties for each light
        if (lightNumber == 1) {
            lightingShader.setVec3("pointLights[0].position", position);
            lightingShader.setVec3("pointLights[0].ambient", ambientOn * ambient);  //turn off hoile ambientOn 0, so ultimately ambient = 0 . complemete shutdown, we can add some number to make sure there is little light/color, even if everything is turned off, its called ambient light
            lightingShader.setVec3("pointLights[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[0].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[0].k_c", k_c);
            lightingShader.setFloat("pointLights[0].k_l", k_l);
            lightingShader.setFloat("pointLights[0].k_q", k_q);
        }

        else if (lightNumber == 2)
        {
            lightingShader.setVec3("pointLights[1].position", position);
            lightingShader.setVec3("pointLights[1].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[1].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[1].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[1].k_c", k_c);
            lightingShader.setFloat("pointLights[1].k_l", k_l);
            lightingShader.setFloat("pointLights[1].k_q", k_q);
        }
        
    }
    void turnOff() //light off
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private: //for light on-off
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* pointLight_h */
