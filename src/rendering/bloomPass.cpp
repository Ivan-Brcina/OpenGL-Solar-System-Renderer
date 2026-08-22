#include "../../headers/rendering/bloomPass.hpp"
#include <iostream>

BloomPass::BloomPass(GLFWwindow* window, GLuint vao, GLuint vbo)
  : window_(window), quadVAO_(vao), quadVBO_(vbo)  {

  setupPingPong();
  blurShader_ = std::make_unique<Shader>(
      "shaders/blurShader.vert",
      "shaders/blurShader.frag"
      );
  blurShader_->setUniformi("brightTexture", 0);

}

void BloomPass::setupPingPong() {
  int w, h;
  glfwGetFramebufferSize(window_, &w, &h);
  glGenFramebuffers(2, pingPongFBOs_);
  glGenTextures(2, pingPongColors);

  for(int i = 0; i < 2; i++) {
    glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBOs_[i]);
    glBindTexture(GL_TEXTURE_2D, pingPongColors[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingPongColors[i], 0
    );

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "HDR FBO incomplete: 0x" << std::hex << status << std::endl;
  }

}

void BloomPass::render(Sun& sun, GLuint brightColors) {
  
  bool horizontal = true, first_iteration = true;
  int amount = 20;
  blurShader_->use();
  blurShader_->setUniformf("spread", sun.blurSpread);
  for (int i = 0; i < amount; i++)
  {
      glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBOs_[horizontal]); 
      blurShader_->setUniformi("horizontal", horizontal);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(
          GL_TEXTURE_2D, first_iteration ? brightColors : pingPongColors[!horizontal]
      ); 
      glBindVertexArray(quadVAO_);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
      finalBlurColor = pingPongColors[horizontal];
      horizontal = !horizontal;
      if (first_iteration)
          first_iteration = false;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
