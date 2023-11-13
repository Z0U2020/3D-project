#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>

//some global variables for handling the vertex buffer
GLuint vertexbuffer;
GLuint VertexArrayID;
GLuint vertexbuffer_size;

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;


int main( void ); //<<< main function, called at startup


#endif
