#include "../../headers/scene/celestialBody.hpp"
#include "../../headers/core/application.hpp"
#include <glm/trigonometric.hpp>

Sun::Sun(GLuint vao, unsigned int idxCount) : vao(vao), idxCount(idxCount) {
  center = glm::vec3(0.0f,0.0f,0.0f);
  radius = 1.0f;
  spinSpeed = 180.0f;
  spinAngle = 0.0f;
  blurSpread = 3.0f;
  emissionStrength = 20.0f;
  exposure = 0.25f;
  diffuseTexture = textureFromFile("assets/8k_sun.jpg");
}

Earth::Earth(GLuint vao, unsigned int idxCount) : vao(vao), idxCount(idxCount) {
  center = glm::vec3(0.0f,0.0f,0.0f);
  radius = 0.6f;
  orbitRadius = 100.0f;
  spinSpeed = 3.0f;
  spinAngle = 0.0f;
  orbitSpeed = 20.0f;
  orbitAngle = 0.0f;
  cloudSpeed = 5.0f;
  cloudAngle = 0.0f;
  tilt = 23.44f;
  diffuseTextureDay = textureFromFile("assets/8k_earth_daymap.jpg");
  diffuseTextureNight = textureFromFile("assets/8k_earth_nightmap.jpg");
  diffuseTextureClouds = textureFromFile("assets/8k_earth_clouds.jpg");
  specularMap = textureFromFile("assets/8k_earth_specular_map.png", false);
  normalMap = textureFromFile("assets/8k_earth_normal_map.jpg", false);
  toggleNormalMapping = false;
}

Moon::Moon(GLuint vao, unsigned int idxCount) : vao(vao), idxCount(idxCount) {
  radius = 0.15f;
  orbitRadius = 15.0f;
  orbitAngle = 0.0f;
  spinSpeed = 1.0f;
  orbitSpeed = 90.0f;
  diffuseTexture = textureFromFile("assets/8k_moon.jpg");
}

Stars::Stars(GLuint vao) : vao(vao) {
  std::vector<std::string> paths = {
    "assets/skybox/px.png",
    "assets/skybox/nx.png",
    "assets/skybox/py.png",
    "assets/skybox/ny.png",
    "assets/skybox/pz.png",
    "assets/skybox/nz.png"
  };
  skybox = loadCubemap(paths);
}

glm::mat4 Sun::getModelMatrix() {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(spinAngle), glm::vec3(0.0f,1.0f,0.0f));
  model = glm::scale(model, glm::vec3(radius));
  return model;
}

glm::mat4 Earth::getModelMatrix() {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, center);
  model = glm::rotate(model, glm::radians(tilt), glm::vec3(1.0f,0.0f,0.0f));
  model = glm::rotate(model, glm::radians(spinAngle), glm::vec3(0.0f,1.0f,0.0f));
  model = glm::scale(model, glm::vec3(radius));
  return model;
}

glm::mat4 Moon::getModelMatrix(Earth& earth) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, earth.center);
  model = glm::rotate(model, glm::radians(orbitAngle), glm::vec3(0.0f,1.0f,0.0f));
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, orbitRadius));
  model = glm::scale(model, glm::vec3(radius));
  return model;
}
