#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include "parse_stl.h"

#include "RenderingObject.h"

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;

//global variables to handle the MVP matrix
GLuint View_Matrix_ID;
glm::mat4 V;
GLuint Projection_Matrix_ID;
glm::mat4 P;
GLuint Model_Matrix_ID;

RenderingObject ground;
RenderingObject sphere1;
RenderingObject sphere2;
RenderingObject sphere3;
RenderingObject sphere4;

float curr_x;
float curr_y;
float curr_angle;

float cam_z;
float sphere_x = 0.0f;
float cam_y;
float cam_angle;
float rotate_angle;

int numRings = 10;
int numSegments = 10;


int main( void ); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool initializeMVPTransformation();
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW

void updateMovingObjectTransformation();


#endif
