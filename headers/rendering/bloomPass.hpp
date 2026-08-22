#pragma once
#include "../scene/celestialBody.hpp"
#include "../core/shaders.hpp"
#include <memory>

class BloomPass {
  private:
    GLFWwindow* window_;
    std::unique_ptr<Shader> blurShader_;
    GLuint quadVAO_, quadVBO_;
    GLuint pingPongFBOs_[2];
    GLuint pingPongColors[2];
    GLuint finalBlurColor = 0;

    void setupPingPong();
  public:
    BloomPass(GLFWwindow* window, GLuint vao, GLuint vbo);
    BloomPass() = default;

    GLuint getFinalBlurColor() const {return finalBlurColor;}

    void render(Sun& sun, GLuint brightColors);
};
