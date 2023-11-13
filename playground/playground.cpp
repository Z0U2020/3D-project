// Include necessary headers
#include "playground.h"
#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <vector>

// Set the depth level for the Sierpinski triangle

// Vertex shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
out vec3 vertexColor;

uniform float iTime; // Time uniform variable

void main() {
    // Set the vertex position
    gl_Position = vec4(aPos, 0.0, 1.0);

    // Modify colors using sine and cosine functions based on iTime
    float r = 0.5 + 0.5 * sin(iTime);
    float g = 0.5 + 0.5 * cos(iTime * 0.5);
    float b = 0.5 + 0.5 * sin(iTime * 0.3);

    // Set the output color for the fragment shader
    vertexColor = vec3(r, g, b);
}
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    // Set the fragment color based on the vertex color
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Vector to store Sierpinski triangle vertices
std::vector<float> sierpinskiVertices;


// Function to generate Sierpinski triangle
void generateSierpinski(std::vector<Point>& vertices, int depth, const Point& p1, const Point& p2, const Point& p3) {
    if (depth == 0) {
        // Add the vertices when depth is 0
        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
    } else {
        // Calculate midpoints of the sides
        Point mid1((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
        Point mid2((p2.x + p3.x) / 2, (p2.y + p3.y) / 2);
        Point mid3((p3.x + p1.x) / 2, (p3.y + p1.y) / 2);

        // Recursively generate smaller triangles
        generateSierpinski(vertices, depth - 1, p1, mid1, mid3);
        generateSierpinski(vertices, depth - 1, mid1, p2, mid2);
        generateSierpinski(vertices, depth - 1, mid3, mid2, p3);
    }
}

// Callback function for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Set the viewport to match the new window dimensions
    glViewport(0, 0, width, height);
}

// Function to process input (e.g., closing the window)
void processInput(GLFWwindow* window) {
    // Check if the ESC key is pressed; if so, close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Variables for fixed time step rendering
float accumulator;
const double fixedDeltaTime = 0.016;

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Sierpinski Triangle", nullptr, nullptr);
    if (window == nullptr) {
        // Handle window creation failure
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        // Handle GLEW initialization failure
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set viewport and callback for window resizing
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compile and link shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up initial triangle
    std::vector<Point> initialTriangle;

    // Generate Sierpinski triangle vertices
    Point p1(-0.5f, -0.5f);
    Point p2(0.5f, -0.5f);
    Point p3(0.0f, 0.5f);
    generateSierpinski(initialTriangle, _DEPTH, p1, p2, p3);
    static const int vertexArrSize = initialTriangle.size() * 2;
    std::vector<float> vertexArray(vertexArrSize);
    for (int i = 0; i < initialTriangle.size() * 2; i += 2) {
        vertexArray[i] = initialTriangle[i / 2].x;
        vertexArray[i + 1] = initialTriangle[i / 2].y;
    }

    // Set up Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, initialTriangle.size() * sizeof(float) * 2, vertexArray.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    float oldTime = glfwGetTime();

    int i = 1;
    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set time uniform in the shader
        glUseProgram(shaderProgram);
        int timeUniform = glGetUniformLocation(shaderProgram, "iTime");
        glUniform1f(timeUniform, time);

        // Update vertex buffer data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (3 * i) * sizeof(float) * 2, vertexArray.data(), GL_DYNAMIC_DRAW);

        // Draw current triangle
        glUseProgram(shaderProgram);
        glUniform1f(timeUniform, time);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, initialTriangle.size());

        if (i < initialTriangle.size() / 3) {
            i++;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
