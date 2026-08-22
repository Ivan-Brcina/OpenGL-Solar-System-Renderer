#include "../../headers/rendering/scenePass.hpp"

#include <iostream>

ScenePass::ScenePass(GLFWwindow* window) : window(window) {
  setupHdrFramebuffer();
  hdrShader = std::make_unique<Shader>(
      "shaders/hdrShader.vert",
      "shaders/hdrShader.frag");
  hdrShader->setUniformi("sunTex", 0);
  
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  GLuint colorBufferEnums[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2, colorBufferEnums);
  
  skyboxShader = std::make_unique<Shader>(
      "shaders/skyboxShader.vert",
      "shaders/skyboxShader.frag");
  skyboxShader->setUniformi("skybox", 0);

  earthShader = std::make_unique<Shader>(
      "shaders/earthShader.vert",
      "shaders/earthShader.frag");
  earthShader->setUniformi("dayDiffuse", 0);
  earthShader->setUniformi("specularMap", 1);
  earthShader->setUniformi("nightDiffuse", 2);
  earthShader->setUniformi("depthMap", 3);

  cloudsShader = std::make_unique<Shader>(
      "shaders/cloudsShader.vert",
      "shaders/cloudsShader.frag"); 
  cloudsShader->setUniformi("cloudMap", 0);
  cloudsShader->setUniformi("depthMap", 1);

  moonShader = std::make_unique<Shader>(
      "shaders/moonShader.vert",
      "shaders/moonShader.frag"); 
  moonShader->setUniformi("moonDiffuse", 0);
  moonShader->setUniformi("depthMap", 1);
}

void ScenePass::setupHdrFramebuffer() {
  int windowW = 0;
  int windowH = 0;
  glfwGetFramebufferSize(window, &windowW, &windowH);
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glGenTextures(2, colorBuffers);
  glGenRenderbuffers(1, &RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowW, windowH);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
 
  for (int i = 0; i < 2; i++) {
    glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowW, windowH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
  }

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "HDR FBO incomplete: 0x" << std::hex << status << std::endl;
}

void ScenePass::renderSun(Sun& sun, const glm::mat4& view, const glm::mat4& projection,
                          std::unique_ptr<Camera>& camera) {
  hdrShader->use();
  hdrShader->setUniformMat4("view", view);
  hdrShader->setUniformMat4("projection", projection);
  hdrShader->setUniformMat4("model", sun.getModelMatrix());
  hdrShader->setUniformf("emissionStrength", sun.emissionStrength);
  hdrShader->setUniformVec3("cameraPos", camera->getPosition());

  glBindVertexArray(sun.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sun.diffuseTexture);
    glDrawElements(GL_TRIANGLES, sun.idxCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void ScenePass::renderEarth(Earth& earth, 
                            const glm::mat4& view, const glm::mat4& projection, 
                            GLuint depthMap, std::unique_ptr<Camera>& camera,
                            const glm::mat4& lightSpaceMatrix) {
  earthShader->use();
  earthShader->setUniformMat4("view", view);
  earthShader->setUniformMat4("projection", projection);
  earthShader->setUniformMat4("model", earth.getModelMatrix());
  earthShader->setUniformMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat4(earth.getModelMatrix()))));
  earthShader->setUniformVec3("lightDirection", glm::normalize(-earth.center));
  earthShader->setUniformVec3("cameraPos", camera->getPosition());
  earthShader->setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

  glBindVertexArray(earth.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earth.diffuseTextureDay);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, earth.specularMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, earth.diffuseTextureNight);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDrawElements(GL_TRIANGLES, earth.idxCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void ScenePass::renderClouds(Earth& earth, const glm::mat4& view, const glm::mat4& projection,
                             GLuint depthMap, const glm::mat4& lightSpaceMatrix) {
  cloudsShader->use();
  cloudsShader->setUniformMat4("view", view);
  cloudsShader->setUniformMat4("projection", projection);
  glm::mat4 model = glm::scale(earth.getModelMatrix(), glm::vec3(1.01));
  model = glm::rotate(model, glm::radians(-earth.cloudAngle), glm::vec3(0.0f,1.0f,0.0f));
  cloudsShader->setUniformMat4("model", model);
  cloudsShader->setUniformMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat4(model))));
  cloudsShader->setUniformVec3("lightDirection", glm::normalize(-earth.center));
  cloudsShader->setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);
  glBindVertexArray(earth.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earth.diffuseTextureClouds);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDrawElements(GL_TRIANGLES, earth.idxCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glDepthMask(GL_TRUE);
}

void ScenePass::renderMoon(Earth& earth, Moon& moon, 
                          const glm::mat4& view, const glm::mat4& projection,
                          const GLuint depthMap, const glm::mat4& lightSpaceMatrix) {
  moonShader->use();
  moonShader->setUniformMat4("view", view);
  moonShader->setUniformMat4("projection", projection);
  moonShader->setUniformMat4("model", moon.getModelMatrix(earth));
  moonShader->setUniformMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat4(moon.getModelMatrix(earth)))));
  moonShader->setUniformVec3("lightDirection", glm::normalize(-earth.center));
  moonShader->setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

  glBindVertexArray(moon.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moon.diffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDrawElements(GL_TRIANGLES, moon.idxCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void ScenePass::renderSkyBox(Stars& stars, const glm::mat4& view, const glm::mat4& projection) {
  skyboxShader->use();
  skyboxShader->setUniformMat4("view", glm::mat4(glm::mat3(view)));
  skyboxShader->setUniformMat4("projection", projection);

  glBindVertexArray(stars.vao);
    glDepthMask(GL_FALSE);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, stars.skybox);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
  glBindVertexArray(0);
}

void ScenePass::render(Sun& sun, Earth& earth, Moon& moon, Stars& stars,
                       std::unique_ptr<Camera>& camera, GLuint depthMap, 
                       const glm::mat4& lightSpaceMatrix) {

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    int windowW = 0;
    int windowH = 0;
    glfwGetFramebufferSize(window, &windowW, &windowH);

    glViewport(0, 0, windowW, windowH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera->getView();
    glm::mat4 projection = camera->getProjection(windowW, windowH);

    renderEarth(earth, view, projection, depthMap, camera, lightSpaceMatrix);
    renderClouds(earth, view, projection, depthMap, lightSpaceMatrix);
    renderMoon(earth, moon, view, projection, depthMap, lightSpaceMatrix);
    renderSun(sun, view, projection, camera);
    renderSkyBox(stars, view, projection);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
