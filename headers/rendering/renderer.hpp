#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "shadowPass.hpp"
#include "scenePass.hpp"
#include "bloomPass.hpp"
#include "finalPass.hpp"

class Renderer {
  private:
    ShadowPass shadowPass;
    ScenePass scenePass;
    BloomPass bloomPass;
    FinalPass finalPass;

    void setupQuadVAO(GLuint& vao, GLuint& vbo);

  public:
    Renderer(GLFWwindow* window);
    Renderer() = default;

    void render(Sun& sun, Earth& earth, Moon& moon, Stars& stars, 
                std::unique_ptr<Camera>& camera);
};

