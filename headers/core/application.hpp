#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "../scene/camera.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
};

GLFWwindow* openGlInit();
void setupDearImGui(GLFWwindow* window);
void processInput(GLFWwindow* window, std::unique_ptr<Camera>& camera);
GLuint textureFromFile(const char* path, bool isColorTex = true);
void generateUVSphere(float radius, unsigned int sectors, unsigned int stacks,
                                     std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices);
GLuint loadCubemap(std::vector<std::string>& paths);
void setupDebuggingMessages();
