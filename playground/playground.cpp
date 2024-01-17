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
    // Red
    glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f);

    // Blue
    glm::vec3 blueColor = glm::vec3(0.0f, 0.0f, 1.0f);

    // Green
    glm::vec3 greenColor = glm::vec3(0.0f, 1.0f, 0.0f);

    // White
    glm::vec3 whiteColor = glm::vec3(1.0f, 1.0f, 1.0f);

    // Orange
    glm::vec3 orangeColor = glm::vec3(1.0f, 0.5f, 0.0f);

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
    else if (glfwGetKey(window, GLFW_KEY_RIGHT)) cam_lookAt_x += 1.0;
    else if (glfwGetKey(window, GLFW_KEY_LEFT)) cam_lookAt_x -= 1.0;
    else if (glfwGetKey(window, GLFW_KEY_UP)) cam_z += 0.1;
    else if (glfwGetKey(window, GLFW_KEY_DOWN)) cam_z -= 0.1;
    else if (glfwGetKey(window, GLFW_KEY_Q)) cam_angle += 0.1;
    else if (glfwGetKey(window, GLFW_KEY_E)) cam_angle -= 0.1;

    else if (glfwGetKey(window, GLFW_KEY_R)) curr_x = 0,curr_y = 0,cam_z = 300, cam_lookAt_x = 0, cam_angle = 0, rotate_angle = 0;
  sphere1.M = glm::rotate(glm::mat4(1.0f), cam_angle, { 0.0f, 1.0f, 0.0f });

  initializeMVPTransformation();

  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform
  glUniformMatrix4fv(View_Matrix_ID, 1, GL_FALSE, &V[0][0]);
  glUniformMatrix4fv(Projection_Matrix_ID, 1, GL_FALSE, &P[0][0]);
/*  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &ground.M[0][0]);
  ground.DrawObject();*/
//####################### FIRST BUNDLE ###################
    updateMovingObjectTransformation();
    sphere1.color = greenColor;
    glUniform3fv(Color_Matrix_ID, 1, &sphere1.color[0]);
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
    //bundle1_x += 1.0f;

//####################### SECOND BUNDLE ###################
    sphere5.color = blueColor;
    glUniform3fv(Color_Matrix_ID, 1, &sphere5.color[0]);
    updateMovingObjectTransformation();
    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere5.M[0][0]);
    sphere1.DrawObject();

    updateMovingObjectTransformation();
    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere6.M[0][0]);
    sphere2.DrawObject();

    updateMovingObjectTransformation();
    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere7.M[0][0]);
    sphere3.DrawObject();

    updateMovingObjectTransformation();
    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere8.M[0][0]);
    sphere4.DrawObject();
    //bundle2_x += 1.0f;

    //####################### THIRD BUNDLE ###################
    sphere9.color = redColor;
    glUniform3fv(Color_Matrix_ID, 1, &sphere9.color[0]);
    updateMovingObjectTransformation();

    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere9.M[0][0]);
    sphere1.DrawObject();

    updateMovingObjectTransformation();
    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere10.M[0][0]);
    sphere2.DrawObject();

    updateMovingObjectTransformation();
    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere11.M[0][0]);
    sphere3.DrawObject();

    updateMovingObjectTransformation();
    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere12.M[0][0]);
    sphere4.DrawObject();
    //bundle3_x += 1.0f;

    //####################### REAL EARTH ###################
    updateMovingObjectTransformation();
    glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &realEarth.M[0][0]);
    realEarth.DrawObject();

    rotate_angle += 1.0f;
  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void updateMovingObjectTransformation()
{

    //####################### FIRST BUNDLE ###################
    // Reset the model matrices to the identity matrix
    glm::mat4 rotationCenterY = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 1.0f, 0.0f});

    glm::mat4 translate1 = glm::translate(glm::mat4(1.0f), bundle1_pos);
    sphere1.M = rotationCenterY * translate1;

    // Apply rotation to sphere2 around the x-axis relative to sphere1
    glm::mat4 rotate2 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 0.0f, 1.0f});
    glm::mat4 translate2 = glm::translate(glm::mat4(1.0f), {  150.0f, 0.0f, 0.0f }); // Offset from sphere1
    sphere2.M = rotationCenterY * translate1 * rotate2 * translate2;

    // Apply rotation to sphere3 around the x-axis relative to sphere1
    glm::mat4 rotate3 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {1.0f, 0.0f, 0.0f});
    glm::mat4 translate3 = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f,  150.0f }); // Offset from sphere1
    sphere3.M = rotationCenterY * translate1 * rotate3 * translate3;

    // Apply rotation to sphere4 around the x-axis relative to sphere1
    glm::mat4 rotate4 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 1.0f, 0.0f});
    glm::mat4 translate4 = glm::translate(glm::mat4(1.0f), { - 150.0f, 0.0f, 0.0f }); // Offset from sphere1
    sphere4.M = rotationCenterY * translate1 * rotate4 * translate4;


    //####################### SECOND BUNDLE ###################
    // Reset the model matrices to the identity matrix

    glm::mat4 rotationCenterX = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 0.0f, 1.0f});

    glm::mat4 translate5 = glm::translate(glm::mat4(1.0f), bundle2_pos);
    sphere5.M = rotationCenterX * translate5;

    // Apply rotation to sphere2 around the x-axis relative to sphere1
    glm::mat4 rotate6 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 0.0f, 1.0f});
    glm::mat4 translate6 = glm::translate(glm::mat4(1.0f), { 150.0f, 0.0f, 0.0f }); // Offset from sphere1
    sphere6.M = rotationCenterX * translate5 * rotate6 * translate6;

    // Apply rotation to sphere3 around the x-axis relative to sphere1
    glm::mat4 rotate7 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {1.0f, 0.0f, 0.0f});
    glm::mat4 translate7 = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f,  150.0f }); // Offset from sphere1
    sphere7.M = rotationCenterX * translate5 * rotate7 * translate7;

    // Apply rotation to sphere4 around the x-axis relative to sphere1
    glm::mat4 rotate8 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 1.0f, 0.0f});
    glm::mat4 translate8 = glm::translate(glm::mat4(1.0f), {-150.0f, 0.0f, 0.0f }); // Offset from sphere1
    sphere8.M = rotationCenterX * translate5 * rotate8 * translate8;


    //####################### THIRD BUNDLE ###################
    // Reset the model matrices to the identity matrix

    glm::mat4 rotationCenterZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {1.0f, 0.0f, 0.0f});
    glm::mat4 translate9 = glm::translate(glm::mat4(1.0f), bundle3_pos);
    sphere9.M = rotationCenterZ * translate9;

    // Apply rotation to sphere2 around the x-axis relative to sphere1
    glm::mat4 rotate10 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 0.0f, 1.0f});
    glm::mat4 translate10 = glm::translate(glm::mat4(1.0f), {150.0f, 0.0f, 0.0f }); // Offset from sphere1
    sphere10.M = rotationCenterZ * translate9 * rotate10 * translate10;

    // Apply rotation to sphere3 around the x-axis relative to sphere1
    glm::mat4 rotate11 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {1.0f, 0.0f, 0.0f});
    glm::mat4 translate11 = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f,150.0f }); // Offset from sphere1
    sphere11.M = rotationCenterZ * translate9 * rotate11 * translate11;

    // Apply rotation to sphere4 around the x-axis relative to sphere1
    glm::mat4 rotate12 = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), {0.0f, 1.0f, 0.0f});
    glm::mat4 translate12 = glm::translate(glm::mat4(1.0f), {-150.0f, 0.0f, 0.0f }); // Offset from sphere1
    sphere12.M = rotationCenterZ * translate9 * rotate12 * translate12;

    //####################### REAL EARTH ###################

    glm::mat4 translate13 = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    //glm::mat4 scale13 = glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f });
    glm::mat4 rotate13 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {0.0f, 0.0f, 1.0f});
    realEarth.M = rotate12 * translate13;
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
    Color_Matrix_ID = glGetUniformLocation(programID, "inColor");

    // Projection matrix : 45? Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);

    // Update the camera translation based on user input (curr_x, curr_y)
    // Orbit around the cube position (make the cube the center of rotation)
    float radius = cam_z; // Set a radius for the orbit
    float cameraX = radius * sin(curr_x);
    float cameraY = radius * sin(curr_y);
    float cameraZ = radius * cos(curr_x);

    glm::vec3 cameraPosition = glm::vec3(cameraX+cam_lookAt_x, cameraY, cameraZ+800);

    // Apply continuous rotation to the camera around the cube
    glm::vec3 upVector(0, 1, 0); // Up direction remains constant

    V = glm::lookAt(
            cameraPosition, // Camera position orbiting around the cube
            glm::vec3{cam_lookAt_x, 0.0f, 0.0f},   // Camera looks at the cube
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

  //####################### SECOND OBJECT: first bundle ###################
  sphere1 = RenderingObject();
  sphere1.InitializeVAO();
  sphere1.LoadSTLSphere("Sphere.stl");

  sphere2 = RenderingObject();
  sphere2.InitializeVAO();
  sphere2.LoadSTLSphere("Sphere.stl");

  sphere3 = RenderingObject();
  sphere3.InitializeVAO();
  sphere3.LoadSTLSphere("Sphere.stl");

  sphere4 = RenderingObject();
  sphere4.InitializeVAO();
  sphere4.LoadSTLSphere("Sphere.stl");

    //####################### SECOND OBJECT: second bundle ###################

    sphere5 = RenderingObject();
    sphere5.InitializeVAO();
    sphere5.LoadSTLSphere("Sphere.stl");

    sphere6 = RenderingObject();
    sphere6.InitializeVAO();
    sphere6.LoadSTLSphere("Sphere.stl");

    sphere7 = RenderingObject();
    sphere7.InitializeVAO();
    sphere7.LoadSTLSphere("Sphere.stl");

    sphere8 = RenderingObject();
    sphere8.InitializeVAO();
    sphere8.LoadSTLSphere("Sphere.stl");

    //####################### SECOND OBJECT: third bundle ###################

    sphere9 = RenderingObject();
    sphere9.InitializeVAO();
    sphere9.LoadSTLSphere("Sphere.stl");

    sphere10 = RenderingObject();
    sphere10.InitializeVAO();
    sphere10.LoadSTLSphere("Sphere.stl");

    sphere11 = RenderingObject();
    sphere11.InitializeVAO();
    sphere11.LoadSTLSphere("Sphere.stl");

    sphere12 = RenderingObject();
    sphere12.InitializeVAO();
    sphere12.LoadSTLSphere("Sphere.stl");

    realEarth = RenderingObject();
    realEarth.InitializeVAO();
    realEarth.LoadSTLSphere("flatearth-binary.stl");

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


