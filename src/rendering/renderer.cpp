#include "../../headers/rendering/renderer.hpp"

Renderer::Renderer(GLFWwindow* window) {
  GLuint vao, vbo;
  setupQuadVAO(vao, vbo);
  shadowPass = ShadowPass(window);
  scenePass  = ScenePass(window);
  bloomPass  = BloomPass(window, vao, vbo);
  finalPass  = FinalPass(window, vao, vbo);
}

void Renderer::setupQuadVAO(GLuint& vao, GLuint& vbo) {
  std::vector<float> quadVertices = {
   // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
  };
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), quadVertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glBindVertexArray(0);
}

void Renderer::render(Sun& sun, Earth& earth, Moon& moon, Stars& stars,
                      std::unique_ptr<Camera>& camera) {

  shadowPass.render(earth, moon);
  scenePass.render(sun, earth, moon, stars, camera,
                   shadowPass.getDepthMap(), shadowPass.getLightSpaceTransform()); 
  bloomPass.render(sun, scenePass.getBrightColorBuffer());
  finalPass.render(scenePass.getHDRColorBuffer(), bloomPass.getFinalBlurColor(), sun.exposure);

  // GLenum err = glGetError();
  // if (err != GL_NO_ERROR) {
  //   std::cout << "GL error in Renderer::render -> " << err << std::endl;
  // }

}
