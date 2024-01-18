#include "playground.h"

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

// Define cube position
glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

  initializeMVPTransformation();
  //initial ball pos
  s_pos_x = 0;
  s_pos_y = 0;
  s_pos_z = 0;

  //random values for the ball direction with different speeds
  s_dir_x = static_cast<float>(std::rand() % 401) / 2000.0f - 0.1f; // Random value between -0.05 and 0.05
  s_dir_y = static_cast<float>(std::rand() % 401) / 2000.0f - 0.1f; // Random value between -0.05 and 0.05
  s_dir_z = static_cast<float>(std::rand() % 401) / 2000.0f - 0.1f; // Random value between -0.05 and 0.05

  //initial cam pos
  cam_x = 0;
  cam_y = 0;
  cam_z = 100;

  // Define an initial speed
  speed = 1.0;


    // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  glColor3f(0.0f ,0.0f ,0.0f );
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

    if (glfwGetKey(window, GLFW_KEY_W)) cam_y+=0.1;
    else if (glfwGetKey(window, GLFW_KEY_S)) cam_y-=0.1;
    else if (glfwGetKey(window, GLFW_KEY_A)) cam_x-=0.1;
    else if (glfwGetKey(window, GLFW_KEY_D)) cam_x+=0.1;
    else if (glfwGetKey(window, GLFW_KEY_DOWN)) cam_z += 1.5;
    else if (glfwGetKey(window, GLFW_KEY_UP)) cam_z -= 1.5;
    else if (glfwGetKey(window, GLFW_KEY_R)) s_pos_x = 0,s_pos_y = 0,s_pos_z = 0,cam_x = 0,cam_y = 0,cam_z = 100, speed =1.0 ;

    initializeMVPTransformation();
    // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
  glUniformMatrix4fv(View_Matrix_ID, 1, GL_FALSE, &V[0][0]);
  glUniformMatrix4fv(Projection_Matrix_ID, 1, GL_FALSE, &P[0][0]);
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &box.M[0][0]);
  box.DrawObject();

  updataMovingObjectTransformation();
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &sphere.M[0][0]);
  sphere.DrawObject();

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}
//box boundary
const float boundingBoxMinX = -10.0f;
const float boundingBoxMaxX = 10.0f;
const float boundingBoxMinY = -10.0f;
const float boundingBoxMaxY = 10.0f;
const float boundingBoxMinZ = -10.0f;
const float boundingBoxMaxZ = 10.0f;

void updataMovingObjectTransformation()

{

    // Update the position based on the direction and speed
    s_pos_x += s_dir_x * speed;
    s_pos_y += s_dir_y * speed;
    s_pos_z += s_dir_z * speed;

    // Check if the sphere hits the bounding box, considering the radius 1,0
    if (s_pos_x - 1.0f <= boundingBoxMinX || s_pos_x + 1.0f >= boundingBoxMaxX) {
        s_dir_x = -s_dir_x;
        // Double the speed each time the ball hits a wall
        speed += 0.1;
    }

    if (s_pos_y - 1.0f <= boundingBoxMinY || s_pos_y + 1.0f >= boundingBoxMaxY) {
        s_dir_y = -s_dir_y;
        speed += 0.1;
    }

    if (s_pos_z - 1.0f <= boundingBoxMinZ || s_pos_z + 1.0f >= boundingBoxMaxZ) {
        s_dir_z = -s_dir_z;
        speed += 0.1;
    }

    // Set the initial position of the sphere
    sphere.M = glm::translate(glm::mat4(1.0f), glm::vec3(s_pos_x, s_pos_y, s_pos_z));
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
  window = glfwCreateWindow(1024, 768, "random ball", NULL, NULL);
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

  // Charcoal background
    glClearColor(0.21f, 0.27f, 0.31f, 0.0f);
  return true;
}
bool initializeMVPTransformation()
{
  // Get a handle for our "MVP" uniform
  Model_Matrix_ID = glGetUniformLocation(programID, "M");
  Projection_Matrix_ID = glGetUniformLocation(programID, "P");
  View_Matrix_ID = glGetUniformLocation(programID, "V");

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);

    // Update the camera translation based on user input (cam_x, cam_y)
    // Orbit around the cube position (make the cube the center of rotation)
    float radius = cam_z; // Set a radius for the orbit
    float cameraX = radius * sin(cam_x);
    float cameraY = radius * sin(cam_y);
    float cameraZ = radius * cos(cam_x);

    glm::vec3 cameraPosition = cubePosition + glm::vec3(cameraX, cameraY, cameraZ);

    // Apply continuous rotation to the camera around the cube
    glm::vec3 cameraTarget = cubePosition; // Camera looks at the cube
    glm::vec3 upVector(0, 1, 0); // Up direction remains constant

    V = glm::lookAt(
            cameraPosition, // Camera position orbiting around the cube
            cameraTarget,   // Camera looks at the cube
            upVector        // Up direction remains constant
    );

  return true;

}
bool initializeVertexbuffer()
{
    //####################### FIRST OBJECT: box ###################
    box = RenderingObject();

    // Create vertex data
    box.InitializeVAO();
    box.LoadSTL("box.stl");

    // Scale the box to make it bigger
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    // Rotate the box so the bottom faces you
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // Set the initial position of the sphere in the middle
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(10.0, -10.0, -10.0));

    box.M = translationMatrix * rotationMatrix * scaleMatrix * box.M;

    // Create texture data for all faces
    box.textureSamplerID = glGetUniformLocation(programID, "myTextureSampler");
    float scaling = 5.0f;

    // Adjusted texture coordinates for all vertices of the box
    std::vector<glm::vec2> uvbufferdata;
    for (int i = 0; i < 36; ++i) {
        uvbufferdata.push_back({0.0f, 0.0f});
        uvbufferdata.push_back({0.0f, scaling});
        uvbufferdata.push_back({scaling, scaling});
        uvbufferdata.push_back({0.0f, 0.0f});
        uvbufferdata.push_back({scaling, scaling});
        uvbufferdata.push_back({scaling, 0.0f});
    }
    box.SetTexture(uvbufferdata, "brick_2.bmp");

    //####################### SECOND OBJECT: sphere ###################
    sphere = RenderingObject();
    sphere.InitializeVAO();
    sphere.LoadSTL("sphere.stl");

    return true;
}
bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteVertexArrays(1, &box.VertexArrayID);
  return true;
}
bool closeWindow()
{
  glfwTerminate();
  return true;
}


