#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "bed.h"
#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "SpotLight.h"
#include "walls.h"
#include "bench.h"
#include "swing.h"
#include "stand.h"
#include "cube.h"
#include "stb_image.h"
#include "cylinderStand.h"
#include <iostream>
#include "sphere.h"
struct Point {
    float x;
    float y;
    float z;
};


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void processInput(GLFWwindow* window);
// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 2.5f+5.0f, 40.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);


// positions of the point lights

glm::vec3 pointLightPositions[] = {
    glm::vec3(-5.5f+19.0f,  12.15f,  -6.6+0.25f+20.0f),
    glm::vec3(40.5f,  12.15f,   -6.6 +0.25f+20.0f),
    glm::vec3(5.5f,  6.15f,   -8.6 + 0.25f),
    
};

PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
        // ambient
    0.3f, 0.3f, 0.3f,     // diffuse
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.014f,  //k_l
    0.0007, //k_q
    1       // light number
);


PointLight pointlight3(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.35f,  //k_l
    0.44f, //k_q
    2       // light number
);

PointLight pointlight4(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.35f,  //k_l
    0.44f, //k_q
    2       // light number
);



DirLight dirLight(
        -0.2f, -1.0f, -0.3f,
        0.05f, 0.05f, 0.05f,
        .5f, 0.5f, 0.5f,     // diffuse
        0.5f, 0.5f, 0.5f
);


SpotLight spotLight(
    -3.50f, 4.0f, -2.0f,
    0.6f, -1.0f, 0.5f,
    0.5f, 0.0f, 0.5f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,
    0.09f,
    0.032f,
    12.5f,
    15.0f
);



// light settings
bool PointToggle1 = true;
bool PointToggle3 = true;
bool PointToggle4 = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 1.5f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, //back side of cube, surface normal on -z 
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //cylinder sadi points
    std::vector<Point> cylinder;
    //cylinder.push_back({ 0, 0, -8 });
    float radius1 = 2.0;
    float radius = 1.0;
    for (float theta = 0; theta <= 360; theta += 1) {
        float x = radius * glm::cos(glm::radians(theta));
        float y = radius * glm::sin(glm::radians(theta));

        float x1 = radius1 * glm::cos(glm::radians(theta));
        float y1 = radius1 * glm::sin(glm::radians(theta));

        cylinder.push_back({ x, y, 0.0f });
        cylinder.push_back({ x1, y1, -2.0f }); // normal cylinder
        //cylinder.push_back({ x * .5f, y * .5f, -8.0f }); // cylinder with different circle radius
    }

    unsigned int CVBO, CVAO;
    glGenVertexArrays(1, &CVAO);
    glGenBuffers(1, &CVBO);

    glBindVertexArray(CVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct Point) * cylinder.size(), cylinder.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)0);
    glEnableVertexAttribArray(0);
    //end of sadi cylinder point

    //CYLINDER CIRCLE POINTS START
    std::vector<Point> cylindercircle;
    cylindercircle.push_back({ 0, 0, -8 });
    float radiusC = 6.0;
    //float radius = 1.0;
    for (float theta = 0; theta <= 360; theta += 1) {
        float x = radiusC * glm::cos(glm::radians(theta));
        float y = radiusC * glm::sin(glm::radians(theta));

        //float x1 = radius1 * glm::cos(glm::radians(theta));
        //float y1 = radius1 * glm::sin(glm::radians(theta));

        cylindercircle.push_back({ x, y, -8.0f });
        //cylinder.push_back({ x1, y1, -2.0f }); // normal cylinder
        //cylinder.push_back({ x * .5f, y * .5f, -8.0f }); // cylinder with different circle radius
    }

    unsigned int CcVBO, CcVAO;
    glGenVertexArrays(1, &CcVAO);
    glGenBuffers(1, &CcVBO);

    glBindVertexArray(CcVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CcVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct Point) * cylindercircle.size(), cylindercircle.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)0);
    glEnableVertexAttribArray(0);
    //end of sadi cylinderCIRCLE point







    // cylinder maku points start
    std::vector<Point> cylinderMaku;
    cylinderMaku.push_back({ 0, 0, -15 });
    float radiusM = 6.0;
    //float radius = 1.0;
    for (float theta = 0; theta <= 360; theta += 1) {
        float x = radiusM * glm::cos(glm::radians(theta));
        float y = radiusM * glm::sin(glm::radians(theta));

        //float x1 = radius1 * glm::cos(glm::radians(theta));
        //float y1 = radius1 * glm::sin(glm::radians(theta));

        cylinderMaku.push_back({ x, y, -8.0f });
        //cylinder.push_back({ x1, y1, -2.0f }); // normal cylinder
        //cylinder.push_back({ x * .5f, y * .5f, -8.0f }); // cylinder with different circle radius
    }

    unsigned int CMVBO, CMVAO;
    glGenVertexArrays(1, &CMVAO);
    glGenBuffers(1, &CMVBO);

    glBindVertexArray(CMVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CMVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct Point) * cylinderMaku.size(), cylinderMaku.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)0);
    glEnableVertexAttribArray(0);
    //end of sadi cylinderMaku point


    //cylinder for rider shade
    std::vector<Point> cylinderShade;
    //cylinder.push_back({ 0, 0, -8 });
    float radiusSh = 3.0;
    for (float theta = 0; theta <= 360; theta += 1) {
        float x = radiusSh * glm::cos(glm::radians(theta));
        float y = radiusSh * glm::sin(glm::radians(theta));

        cylinderShade.push_back({ x, y, 0.0f });
        cylinderShade.push_back({ x, y, -2.0f }); // normal cylinder
        //cylinder.push_back({ x * .5f, y * .5f, -8.0f }); // cylinder with different circle radius
    }

    unsigned int CSVBO, CSVAO;
    glGenVertexArrays(1, &CSVAO);
    glGenBuffers(1, &CSVBO);

    glBindVertexArray(CSVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CSVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct Point) * cylinderShade.size(), cylinderShade.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)0);
    glEnableVertexAttribArray(0);
    //end of cylinderShade point



    
    
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------

    // abrar er vdo dekhe ana code
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    //Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    string diffuseMapPath = "./container2.jpg";
    string specularMapPath = "./container2.jpg";
    string diffuseMapRoad = "./road.bmp";
    string specularMapRoad = "./road.bmp";
    string diffuseMapWater = "./water.bmp";
    string specularMapWater = "./water.bmp";
    string diffuseMapWood = "./wood.bmp";
    string specularMapWood = "./wood.bmp";
    string diffuseMapSeeSaw = "./seesawseat.bmp";
    string specularMapSeeSaw = "./seesawseat.bmp";
    string diffuseMapGrass = "./grass.bmp";
    string specularMapGrass = "./grass.bmp";
    string diffuseMapCement = "./cement.bmp";
    string specularMapCement = "./cement.bmp";
    string diffuseMapBrick = "./brick.bmp";
    string specularMapBrick = "./brick.bmp";
    string diffuseMapBench = "./bench.bmp";
    string specularMapBench = "./bench.bmp";
    string diffuseMapMetal = "./metalPole.bmp";
    string specularMapMetal = "./metalPole.bmp";
    string diffuseMapRope = "./rope.bmp";
    string specularMapRope = "./rope.bmp";
    string diffuseMapSky = "./skyfront1.bmp";
    string specularMapSky = "./skyfront1.bmp";
    string diffuseMapSeat = "./planeSeat.bmp";
    string specularMapSeat = "./planeSeat.bmp";
    string diffuseMapStick = "./planeStick.bmp";
    string specularMapStick = "./planeStick.bmp";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap1 = loadTexture(diffuseMapRoad.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap1 = loadTexture(specularMapRoad.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap2 = loadTexture(diffuseMapWater.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapWater.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap3 = loadTexture(diffuseMapWood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapWood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap4 = loadTexture(diffuseMapSeeSaw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap4 = loadTexture(specularMapSeeSaw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap5 = loadTexture(diffuseMapGrass.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap5 = loadTexture(specularMapGrass.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap6 = loadTexture(diffuseMapCement.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap6 = loadTexture(specularMapCement.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap7 = loadTexture(diffuseMapBrick.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap7 = loadTexture(specularMapBrick.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap8 = loadTexture(diffuseMapBench.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap8 = loadTexture(specularMapBench.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap9 = loadTexture(diffuseMapMetal.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap9 = loadTexture(specularMapMetal.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap10 = loadTexture(diffuseMapRope.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap10 = loadTexture(specularMapRope.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap11 = loadTexture(diffuseMapSky.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap11 = loadTexture(specularMapSky.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap12 = loadTexture(diffuseMapSeat.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap12 = loadTexture(specularMapSeat.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap13 = loadTexture(diffuseMapStick.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap13 = loadTexture(specularMapStick.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Cube road = Cube(diffMap1, specMap1, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube water = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube wood = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube seesaw = Cube(diffMap4, specMap4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube grass = Cube(diffMap5, specMap5, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube cement = Cube(diffMap6, specMap6, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube brick = Cube(diffMap7, specMap7, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube bench = Cube(diffMap8, specMap8, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube metal = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube rope = Cube(diffMap10, specMap10, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube sky = Cube(diffMap11, specMap11, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube seat = Cube(diffMap12, specMap12, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube stick = Cube(diffMap13, specMap13, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //std::cout << diffMap << ' ' << specMap << std::endl;
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube cube1 = Cube();
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        //pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        dirLight.setUpDirLight(lightingShader);
        spotLight.setUpSpotLight(lightingShader);
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        float degree = 0;
        glm::mat4 view = camera.GetViewMatrix();
        float r = glm::length(camera.Position - glm::vec3(view[3]));
        ourShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
       /*m::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
       */
        /*------------------------------INSIDE GROUND----------------------------*/
        glm::mat4 translateMatrix26, rotateXMatrix26, rotateYMatrix26, rotateZMatrix26, scaleMatrix26, model26;
        translateMatrix26 = glm::translate(identityMatrix, glm::vec3(-2, -1.1, -2.0));
        rotateXMatrix26 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix26 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix26 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix26 = glm::scale(identityMatrix, glm::vec3(30.0, 30.0, 0.0));
        model26 = translateMatrix26 * rotateXMatrix26 * rotateYMatrix26 * rotateZMatrix26 * scaleMatrix26;
        lightingShader.setMat4("model", model26);
        grass.drawCubeWithTexture(lightingShaderWithTexture, model26);



        glm::mat4 translateMatrix_b1, rotateXMatrix_b1, rotateYMatrix_b1, rotateZMatrix_b1, scaleMatrix_b1, model_b1;

        /*--------------------------RIVER SIDE FENCHING------------------------*/
        //left side(1-10)
        // stand1
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
       
        //stand2
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, 2.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand3
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, 5.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
       
        //stand4
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10,8.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand5
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, 11.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand6
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, 15.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;

        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand7
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, 18.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand8
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, 21.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand9
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, 24.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand10
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -1.10, 27.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        // Right side(11-20)
        //stand11
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand12
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 2.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand13
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 5.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand14
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 8.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
        
        //stand15
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 11.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand16
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 15.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;

        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand17
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 18.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand18
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 21.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand19
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 24.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand20
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -1.10, 27.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //North side(21-28)
          // stand21
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(2.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand22
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(5.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand23
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(8.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand24
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(11.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand25
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(15.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand26
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(18.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;

        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand27
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand28
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(24.50, -1.10, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 2.50f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //chain1(north)
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, 0.00, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(13.0f, 0.15f, 0.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -0.60, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(13.0f, 0.15f, 0.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, 0.50, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(13.0f, 0.15f, 0.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //chain2(north)
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(15.50, 0.00, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(12.0f, 0.15f, 0.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(15.50, -0.60, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(12.0f, 0.15f, 0.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(15.50, 0.50, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(12.0f, 0.15f, 0.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //chain1(left)
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, 0.00, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 13.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -0.60, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 13.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, 0.50, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 13.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //chain2(left)
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, 0.00, 15.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 12.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, -0.60, 15.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 12.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-1.50, 0.50, 15.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 12.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


        //chain1(right)
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, 0.00, -1.50)); //27.50, -1.10, -1.50
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 13.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -0.60, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 13.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, 0.50, -1.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 13.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //chain2(right)
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, 0.00, 15.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 12.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, -0.60, 15.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 12.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(27.50, 0.50, 15.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.0f, 0.15f, 12.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);




        // also draw the lamp object(s) && draw a cylinder with sadi
        // start of ourshader for complex object
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        //Cylinder Circle code start
        glm::mat4 modelCircle = glm::mat4(1.0f);
 

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);
        //2nd part of cube draw
        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
       
        // surface
        //glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        //translateMatrix26 = glm::translate(identityMatrix, glm::vec3(-2, -1.1, -2.0));
        //scaleMatrix26 = glm::scale(identityMatrix, glm::vec3(30.0, 30.0, .2));
        //modelMatrixForContainer = translateMatrix26 * scaleMatrix26;
        ////modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-0.45f, -0.4f, -2.8f));
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        /*------------------------------road-------------------------*/
       //road1 (x-axis)
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-2.0f, -1.09f, 12.50f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(30.0f, 0.0f, 2.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


        //road2(z-axis)
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(12.50f, -1.09f, -2.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.0f, 0.0f, 30.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        /*--------------------------------POND-----------------------------*/
       //left
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-6.0f, -1.09f, -2.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(4.0f, 0.0f, 30.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        water.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //up
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-6.0f, -1.1f, -6.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(38.0f, 0.0f, 4.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        water.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //right
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(28.0f, -1.09f, -2.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(4.0f, 0.0f, 30.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        water.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        /*--------------------------------OUTSIDE GROUND-----------------------------*/
        //left
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-16.0f, -1.09f, -6.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(10.0f, 0.0f, 34.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        grass.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //up
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-16.0f, -1.1f, -15.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(60.0f, 0.0f, 9.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        grass.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //right
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(32.0f, -1.09f, -6.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(12.0f, 0.0f, 34.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        grass.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        /*----------------------------sky----------------------------------*/
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(-16.0f, -1.1f, -15.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(60.0f, 30.0f, 0.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        sky.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        /*------------------------------seesaw-----------------------------------*/
       //Seesaw1 base
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(2.5, -0.50, 3.5));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(4.0f, .05f, 1.20f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //seesaw1 base pillar back
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(4.750, -1.10, 3.40));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.25f, 0.80f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        brick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //seesaw1 base pillar front
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(4.750, -1.10, 4.70));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.25f, 0.80f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        brick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

      
        //Seesaw2 base
        //translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(3.50, -0.50, 5.0));
        //scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(4.0f, .05f, 1.5f));
        //model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        //road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        ////seesaw2 base pillar back
        //translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(1.30, -1.10, 2.480));
        //scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.25f, 0.50f, 0.05f));
        //model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        //road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        ////seesaw2 base pillar front
        //translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(1.30, -1.10, 2.75));
        //scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.25f, 0.50f, 0.05f));
        //model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        //road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


        /*------------------------RIDE_1---------------------*/
        //stand1
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.50, -1.10, 1.80));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 15.0f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //stand2
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(23.0, -1.10, 1.80));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 15.0f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //upper beam
        //1
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 0.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //2
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 1.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //3
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 2.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //4
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 3.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //5
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 4.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //6
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 5.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //7
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 6.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //8
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 7.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //9
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 8.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //10
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 9.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //11
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 10.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //12
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 11.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //13
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 12.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //14
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(21.5, 13.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.50f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        stick.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        // seat
          // base
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(18.80, 0.0, 2.0));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(6.50f, 0.50f, 1.60f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        seat.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

            //seat uporer part
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(18.80f, 0.0f, 1.90f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(6.50f, 2.10f, .20f));
        //scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(.20f, 2.10f, 5.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        seat.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
          
        //handle left
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(18.60, 1.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(.30f, .30f, 1.80f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


        ////handle right
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(25.20, 1.0, 1.90));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(.30f, .30f, 1.80f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //handle front
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(18.60, 1.0, 3.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(6.90f, .30f, .30f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);



        // translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(18.80, 0.0, 2.0));
        //scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(6.50f, 1.0f, 1.60f));
        //model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        //seat.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


 

        /*----------------------------swing----------------------------*/
       //stand1
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(5.50, -1.10, 6.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 3.0f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
     
        //stand2
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(8.50, -1.10, 6.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 3.0f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
      
        //stand3
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(11.50, -1.10, 6.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.20f, 3.0f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
       
        //upper beam
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(5.0, 1.30, 6.60));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(7.0f, 0.20f, -0.15f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        metal.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
       
        //Swing1
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(5.90, -0.50, 6.750));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(2.0f, .15f, -0.5f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
     
        //swing1 leg1
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(6.05, -0.50, 6.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.05f, 1.9f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //Swing1 leg2
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(7.80, -0.50, 6.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.05f, 1.9f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //Swing2
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(8.90, -0.50, 6.30));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(2.0f, .15f, 0.5f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        road.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //swing2 leg1
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(9.05, -0.50, 6.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.05f, 1.9f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
       
        //Swing2 leg2
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(10.80, -0.50, 6.50));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(-0.05f, 1.9f, 0.05f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;

        rope.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


        /*------------------------------------------------BENCHES-------------------------------------------*/

            // glm::mat4 translateMatrix_b1, rotateXMatrix_b1, rotateYMatrix_b1, rotateZMatrix_b1, scaleMatrix_b1, model_b1;

        //sofa1
            //base
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(4.0f, -1.10f, -1.30f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(3.50f, 1.0f, 1.60f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        wood.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
           
        //sofa1 uporer part
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(4.0f, -1.10f, -1.30f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(3.50f, 2.10f, .20f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        bench.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        // sofa2 
           // base
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(16.0f, -1.10f, -1.30f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(3.50f, 1.0f, 1.60f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        wood.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //sofa2 uporer part
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(16.0f, -1.10f, -1.30f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(3.50f, 2.10f, .20f));
        //scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(.20f, 2.10f, 5.0f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        bench.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


        // sofa3
                  // base
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(0.0f, -1.10f, 5.50f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.60f, 1.0f, 3.50f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        wood.drawCubeWithTexture(lightingShaderWithTexture, model_b1);
          
        //sofa3 uporer part
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(0.0f, -1.10f, 5.50f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.20f, 2.10f, 3.50f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        bench.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


        // sofa4
                  // base
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(0.0f, -1.10f, 19.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.60f, 1.0f, 3.50f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        wood.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

          
        //sofa4 uporer part
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(.0f, -1.10f, 19.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.20f, 2.10f, 3.50f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        bench.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        // sofa5
                   // base
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(24.30f, -1.10f, 5.50f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.60f, 1.0f, 3.50f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        wood.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //sofa5 uporer part
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(25.70f, -1.10f, 5.50f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.20f, 2.10f, 3.50f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        bench.drawCubeWithTexture(lightingShaderWithTexture, model_b1);


        // sofa6
                  // base
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(24.30f, -1.10f, 19.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(1.60f, 1.0f, 3.50f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        wood.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        //sofa6 uporer part
        translateMatrix_b1 = glm::translate(identityMatrix, glm::vec3(25.70f, -1.10f, 19.0f));
        scaleMatrix_b1 = glm::scale(identityMatrix, glm::vec3(0.20f, 2.10f, 3.50f));
        model_b1 = translateMatrix_b1 * scaleMatrix_b1;
        bench.drawCubeWithTexture(lightingShaderWithTexture, model_b1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        pointlight1.turnOn();
        pointlight3.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        pointlight1.turnOff();
        pointlight3.turnOff();

    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        pointlight1.turnDiffuseOn();
        pointlight3.turnDiffuseOn();
        spotLight.turnDiffuseOn();
        dirLight.turnDiffuseOn();

    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        
        pointlight1.turnOff();
        pointlight3.turnOff();
        dirLight.turnOff();
        spotLight.turnOff();

    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        dirLight.turnOn();

    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        dirLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        spotLight.turnOn();

    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        spotLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        pointlight1.turnAmbientOn();
        pointlight3.turnAmbientOn();
        spotLight.turnAmbientOn();
        dirLight.turnAmbientOn();
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        pointlight1.turnSpecularOn();
        pointlight3.turnSpecularOn();
        spotLight.turnSpecularOn();
        dirLight.turnSpecularOn();
    }


 
    


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
