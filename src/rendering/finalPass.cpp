#include "../../headers/rendering/finalPass.hpp"
#include <GLFW/glfw3.h>

FinalPass::FinalPass(GLFWwindow* window, GLuint vao, GLuint vbo)
  : window_(window), quadVAO_(vao), quadVBO_(vbo) {
  screenShader_ = std::make_unique<Shader>(
      "shaders/screenShader.vert",
      "shaders/screenShader.frag");
  screenShader_->use();
  screenShader_->setUniformi("hdrImage", 0);
  screenShader_->setUniformi("blurImage", 1);
}

void FinalPass::render(GLuint hdrTexture, GLuint blurTexture, float exposure) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
    screenShader_->use();
    screenShader_->setUniformf("exposure", exposure);
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(quadVAO_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, blurTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
