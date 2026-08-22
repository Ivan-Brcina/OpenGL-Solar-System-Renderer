#include "../../headers/rendering/shadowPass.hpp"

ShadowPass::ShadowPass(GLFWwindow* window) : window_(window) {
  depthMapWidth = 2048;
  depthMapHeight = 2048;
  setupDepthBuffer();
  depthShader_ = std::make_unique<Shader>(
      "shaders/depthMapShader.vert",
      "shaders/depthMapShader.frag"
      );
  frustumDimension = 5.0f;
}

void ShadowPass::setupDepthBuffer() {
  glGenTextures(1, &depthMap_);
  glBindTexture(GL_TEXTURE_2D, depthMap_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapWidth, depthMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 
  float borderColor[] = {1.0f,1.0f,1.0f,1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glGenFramebuffers(1, &depthFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void ShadowPass::renderEarth(Earth& earth) {
  depthShader_->setUniformMat4("model", earth.getModelMatrix());

  glBindVertexArray(earth.vao);
    glDrawElements(GL_TRIANGLES, earth.idxCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void ShadowPass::renderMoon(Earth& earth, Moon& moon) {
  depthShader_->setUniformMat4("model", moon.getModelMatrix(earth));

  glBindVertexArray(moon.vao);
    glDrawElements(GL_TRIANGLES, moon.idxCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void ShadowPass::renderToDepthMap(Earth& earth, Moon& moon) {
  float near_plane = 1.0f, far_plane = 100.0f;
  glm::mat4 lightProjection = glm::ortho(-frustumDimension, frustumDimension, -frustumDimension, frustumDimension, near_plane, far_plane);
  glm::mat4 lightView = glm::lookAt(earth.center - (earth.center * 0.5f), 
                                    earth.center, 
                                    glm::vec3(0.0f,1.0f,0.0f));
  lightSpaceTransform = lightProjection * lightView;

  depthShader_->use();
  depthShader_->setUniformMat4("lightSpaceTransform", lightSpaceTransform);
  glCullFace(GL_FRONT);
  glEnable(GL_CULL_FACE);

  glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glViewport(0, 0, depthMapWidth, depthMapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderEarth(earth);
    renderMoon(earth, moon);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glCullFace(GL_BACK);
}

void ShadowPass::render(Earth& earth, Moon& moon) {
  renderToDepthMap(earth, moon);
}

