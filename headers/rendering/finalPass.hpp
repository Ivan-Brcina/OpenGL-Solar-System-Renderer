#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "../core/shaders.hpp"
#include <memory>

class FinalPass {
  private:
    GLFWwindow* window_;
    std::unique_ptr<Shader> screenShader_;
    GLuint quadVAO_, quadVBO_;

  public:
    FinalPass(GLFWwindow* window, GLuint vao, GLuint vbo);
    FinalPass() = default;
    void render(GLuint hdrTexture, GLuint blurTexture, float exposure);

};

