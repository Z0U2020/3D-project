#include "playground.h"
#include "../common/shader.hpp"

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders(R"(C:\Users\slama\CLionProjects\OpenGL-Template\3dproj4\playground\SimpleVertexShader.vertexshader)", R"(C:\Users\slama\CLionProjects\OpenGL-Template\3dproj4\playground\SimpleFragmentShader.fragmentshader)");

  initializeMVPTransformation();

  curr_x = 0;
  curr_y = 0;

  cam_z = 300;

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);


  initializeMVPTransformation();

	//start animation loop until escape key is pressed
	do{

    updateAnimationLoop();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}

void updateAnimationLoop()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);


  float scale = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_W)) curr_y+=0.1;
    else if (glfwGetKey(window, GLFW_KEY_S)) curr_y-=0.1;
    else if (glfwGetKey(window, GLFW_KEY_A)) curr_x-=0.1;
    else if (glfwGetKey(window, GLFW_KEY_D)) curr_x+=0.1;
    else if (glfwGetKey(window, GLFW_KEY_O)) cam_z += 1.5;
    else if (glfwGetKey(window, GLFW_KEY_L)) cam_z -= 1.5;
    else if (glfwGetKey(window, GLFW_KEY_R)) curr_x = 0,curr_y = 0,cam_z = 300;
  sphere1.M = glm::rotate(glm::mat4(1.0f), cam_angle, { 0.0f, 1.0f, 0.0f });

  initializeMVPTransformation();

  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform
  glUniformMatrix4fv(View_Matrix_ID, 1, GL_FALSE, &V[0][0]);
  glUniformMatrix4fv(Projection_Matrix_ID, 1, GL_FALSE, &P[0][0]);
/*  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &ground.M[0][0]);
  ground.DrawObject();*/

    updateMovingObjectTransformation();
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere1.M[0][0]);
  sphere1.DrawObject();

    updateMovingObjectTransformation();
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere2.M[0][0]);
  sphere2.DrawObject();

  updateMovingObjectTransformation();
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere3.M[0][0]);
  sphere3.DrawObject();

  updateMovingObjectTransformation();
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere4.M[0][0]);
  sphere4.DrawObject();
    rotate_angle += 1.0f;
    sphere_x += 1.0f;

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void updateMovingObjectTransformation()
{
    // Reset the model matrices to the identity matrix
    glm::mat4 translate1 = glm::translate(glm::mat4(1.0f), { sphere_x, 0.0f, 0.0f });
    sphere1.M = translate1;

    // Apply rotation to sphere2 around the x-axis relative to sphere1
    glm::mat4 rotate2 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 0.0f, 1.0f});
    glm::mat4 translate2 = glm::translate(glm::mat4(1.0f), { 150.0f, 0.0f, 0.0f }); // Offset from sphere1
    sphere2.M = translate1 * rotate2 * translate2;

    // Apply rotation to sphere3 around the x-axis relative to sphere1
    glm::mat4 rotate3 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {1.0f, 0.0f, 0.0f});
    glm::mat4 translate3 = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 150.0f }); // Offset from sphere1
    sphere3.M = translate1 * rotate3 * translate3;

    // Apply rotation to sphere4 around the x-axis relative to sphere1
    glm::mat4 rotate4 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 1.0f, 0.0f});
    glm::mat4 translate4 = glm::translate(glm::mat4(1.0f), { -150.0f, 0.0f, 0.0f }); // Offset from sphere1
    sphere4.M = translate1 * rotate4 * translate4;

}

bool initializeWindow()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Example: simple cube", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
  return true;
}

bool initializeMVPTransformation()
{
    // Get a handle for our "MVP" uniform
    Model_Matrix_ID = glGetUniformLocation(programID, "M");
    Projection_Matrix_ID = glGetUniformLocation(programID, "P");
    View_Matrix_ID = glGetUniformLocation(programID, "V");

    // Projection matrix : 45? Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);

    // Update the camera translation based on user input (curr_x, curr_y)
    // Orbit around the cube position (make the cube the center of rotation)
    float radius = cam_z; // Set a radius for the orbit
    float cameraX = radius * sin(curr_x);
    float cameraY = radius * sin(curr_y);
    float cameraZ = radius * cos(curr_x);

    glm::vec3 cameraPosition = glm::vec3(cameraX, cameraY, cameraZ);

    // Apply continuous rotation to the camera around the cube
    glm::vec3 upVector(0, 1, 0); // Up direction remains constant

    V = glm::lookAt(
            cameraPosition, // Camera position orbiting around the cube
            glm::vec3{0.0f, 0.0f, 0.0f},   // Camera looks at the cube
            upVector        // Up direction remains constant
    );

    return true;

}


bool initializeVertexbuffer()
{
  //####################### FIRST OBJECT: GROUND ###################
  ground = RenderingObject();
  ground.InitializeVAO();

  //create vertex data
  std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
  vertices.push_back({-120,-50,-120});
  vertices.push_back({-120,-50,120 });
  vertices.push_back({ 120,-50,120 });
  vertices.push_back({ -120,-50,-120 });
  vertices.push_back({ 120,-50,120 });
  vertices.push_back({ 120,-50,-120 });
  ground.SetVertices(vertices);

  //create normal data
  std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();
  ground.computeVertexNormalsOfTriangles(vertices, normals);
  ground.SetNormals(normals);

  //create texture data
  ground.textureSamplerID = glGetUniformLocation(programID, "myTextureSampler");

  float scaling = 5.0f;
  std::vector< glm::vec2 > uvbufferdata;
  /*uvbufferdata.push_back({ 0.0f, 0.0f });
  uvbufferdata.push_back({ 0.0f, scaling });
  uvbufferdata.push_back({ scaling, scaling });
  uvbufferdata.push_back({ 0.0f,0.0f });
  uvbufferdata.push_back({ scaling,scaling });
  uvbufferdata.push_back({ scaling,0.0f });*/
  ground.SetTexture(uvbufferdata, R"(C:\Users\slama\CLionProjects\OpenGL-Template\3dproj4\playground\brick_2.bmp)");

  //####################### SECOND OBJECT: SPHERE ###################
  sphere1 = RenderingObject();
  sphere1.InitializeVAO();
  sphere1.LoadSTLSphere(R"(C:\Users\slama\CLionProjects\OpenGL-Template\3dproj4\playground\Sphere.stl)");

  //sphere1.SetTexture(uvbufferdata, R"(C:\Users\slama\CLionProjects\OpenGL-Template\3dproj4\playground\brick_2.bmp)");


  //trying to get the texture to work
 /*   std::vector< glm::vec3 > normalsSphere = std::vector< glm::vec3 >();
    sphere1.computeVertexNormalsOfTriangles(sphere1.vertices, normalsSphere);
    sphere1.SetNormals(normalsSphere);

    sphere1.textureSamplerID = glGetUniformLocation(programID, "myTextureSampler");

  std::vector<glm::vec2> sphere1UVBuffer;  // Adjust this based on your UV coordinates for the sphere
    for (int i = 0; i <= numRings; ++i) {
        for (int j = 0; j <= numSegments; ++j) {
            float u = static_cast<float>(j) / static_cast<float>(numSegments);
            float v = static_cast<float>(i) / static_cast<float>(numRings);
            sphere1UVBuffer.emplace_back(u, v);
        }
    }
  sphere1.SetTexture(sphere1UVBuffer, R"(C:\Users\slama\CLionProjects\OpenGL-Template\3dproj4\playground\ball-texture.bmp)");*/


  sphere2 = RenderingObject();
  sphere2.InitializeVAO();
  sphere2.LoadSTLSphere(R"(C:\Users\slama\CLionProjects\OpenGL-Template\3dproj4\playground\Sphere.stl)");

  sphere3 = RenderingObject();
  sphere3.InitializeVAO();
  sphere3.LoadSTLSphere("Sphere.stl");

  sphere4 = RenderingObject();
  sphere4.InitializeVAO();
  sphere4.LoadSTLSphere("Sphere.stl");


  return true;
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteVertexArrays(1, &ground.VertexArrayID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}


