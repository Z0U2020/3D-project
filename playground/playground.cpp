#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

// Vertex shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 vertexColor;

uniform float iTime; // Time uniform variable

void main() {
    gl_Position = vec4(aPos, 1.0);

    // Modify colors using sine and cosine functions based on iTime
    float r = 0.5 + 0.5 * sin(iTime);
    float g = 0.5 + 0.5 * cos(iTime * 0.5);
    float b = 0.5 + 0.5 * sin(iTime * 0.3);

    vertexColor = vec3(r, g, b);
}
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Vector to store Sierpinski triangle vertices
std::vector<float> sierpinskiVertices;

// Function to generate Sierpinski triangle

void generateSierpinski(std::vector<float> &vertices, int depth) {
    if (depth == 0) {
        for (float vertex : vertices) {
            sierpinskiVertices.push_back(vertex);
        }
    }
    else {
        std::vector<float> v1 = { (vertices[0] + vertices[3]) / 2, (vertices[1] + vertices[4]) / 2,};
        std::vector<float> v2 = { (vertices[0] + vertices[6]) / 2, (vertices[1] + vertices[7]) / 2};
        std::vector<float> v3 = { (vertices[3] + vertices[6]) / 2, (vertices[4] + vertices[7]) / 2};

        generateSierpinski(vertices, depth - 1);

        // Create sub-triangles
        std::vector<float> init_vec = { vertices[0], vertices[1], 0.0f, v1[0], v1[1], 0.0f, v2[0], v2[1], 0.0f };
        generateSierpinski(init_vec, depth - 1);

        std::vector<float> v4 = std::vector<float>{ v1[0], v1[1], 0.0f, vertices[3], vertices[4], 0.0f, v3[0], v3[1], 0.0f };
        std::vector<float> v5 = std::vector<float>{ v2[0], v2[1], 0.0f, v3[0], v3[1], 0.0f, vertices[6], vertices[7], 0.0f };
        generateSierpinski(v4, depth - 1);
        generateSierpinski(v5, depth - 1);
    }
}

// Callback function for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Function to process input (e.g., closing the window)
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float accumulator;
const double fixedDeltaTime = 0.016;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Sierpinski Triangle", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
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
    float initialTriangle[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    // Generate Sierpinski triangle vertices
    std::vector<float> st_vert = std::vector<float>{ initialTriangle[0], initialTriangle[1], 0.0f, initialTriangle[3], initialTriangle[4], 0.0f, initialTriangle[6], initialTriangle[7], 0.0f };
    generateSierpinski(st_vert, 7);

    // Set up Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sierpinskiVertices.size() * sizeof(float), sierpinskiVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float oldTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        float frameTime = time - oldTime;

        oldTime = time;

        accumulator += frameTime;

        while (accumulator >= fixedDeltaTime) {
            accumulator -= fixedDeltaTime;
        }

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        int timeUniform = glGetUniformLocation(shaderProgram, "iTime");
        glUniform1f(timeUniform, time);
        glBindVertexArray(VAO);

        // Adjust line width based on sine function for a dynamic effect
        //glLineWidth(5.0f * abs(sinf(time * 2)));

        // Draw Sierpinski triangle
        glDrawArrays(GL_LINES, 0, sierpinskiVertices.size() / 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
