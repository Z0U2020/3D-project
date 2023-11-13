#ifndef SIERPINSKI_TRIANGLE_H
#define SIERPINSKI_TRIANGLE_H

#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <vector>

// Set the depth level for the Sierpinski triangle
const int _DEPTH = 8;

// Structure for a 2D point
struct Point {
    float x, y;
    Point(float x, float y) : x(x), y(y) {}
};

// Function to generate Sierpinski triangle
void generateSierpinski(std::vector<Point>& vertices, int depth, const Point& p1, const Point& p2, const Point& p3);

// Callback function for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Function to process input (e.g., closing the window)
void processInput(GLFWwindow* window);

// Main rendering function
void renderSierpinskiTriangle();

#endif // SIERPINSKI_TRIANGLE_H
