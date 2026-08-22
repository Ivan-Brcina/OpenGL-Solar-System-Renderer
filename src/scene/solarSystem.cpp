#include "../../headers/scene/solarSystem.hpp"
#include "../../headers/core/mesh.hpp"
#include <GLFW/glfw3.h>

#include "../../headers/imgui/imgui.h"
#include "../../headers/imgui/imgui_impl_glfw.h"
#include "../../headers/imgui/imgui_impl_opengl3.h"

SolarSystem::SolarSystem() {
  window_ = openGlInit();
  setupDebuggingMessages();
  setupDearImGui(window_);
  Mesh meshGenerator;
  unsigned int idxCount;
  idxCount = meshGenerator.createSphere(sphereVAO, sphereVBO, sphereEBO,
                                        5.0f, 128 * 4, 64 * 4); //TODO lower later if bad framerate
  sun = Sun(sphereVAO, idxCount);
  earth = Earth(sphereVAO, idxCount);
  moon = Moon(sphereVAO, idxCount);

  meshGenerator.createCube(skyboxVAO, skyboxVBO);
  stars = Stars(skyboxVAO);

  camera = std::make_unique<Camera>();
  camera->activate();
  renderer = Renderer(window_);
  daysPerSecond = 1.0f;
}

void SolarSystem::updateSpeeds() {
  earth.spinSpeed = 360.0f * daysPerSecond;
  earth.orbitSpeed = (360.0f / 365.0f) * daysPerSecond;
  moon.orbitSpeed = (360.0f / 30.0f) * daysPerSecond;
  sun.spinSpeed = (360.0f / 25.0f) * daysPerSecond;
  earth.cloudSpeed = earth.spinSpeed * 0.1f;
}

void SolarSystem::updatePositions(float deltaTime) {
  updateSpeeds();
  updateEarthPosition(deltaTime);
  updateSunPosition(deltaTime);
  updateMoonPosition(deltaTime);
}

void SolarSystem::updateSunPosition(float deltaTime) {
  sun.spinAngle += sun.spinSpeed * deltaTime;
}

void SolarSystem::updateEarthPosition(float deltaTime) {
  earth.cloudAngle += earth.cloudSpeed * deltaTime;
  earth.spinAngle += earth.spinSpeed * deltaTime;

  earth.orbitAngle -= earth.orbitSpeed * deltaTime;
  earth.center = glm::vec3(glm::cos(glm::radians(earth.orbitAngle)), 
                          0.0f, 
                          glm::sin(glm::radians(earth.orbitAngle))) 
                          * earth.orbitRadius;
}

void SolarSystem::updateMoonPosition(float deltaTime) {
  moon.orbitAngle += moon.orbitSpeed * deltaTime;
}

void SolarSystem::drawGUI() {
  static float frameTimes[100] = {};
  static int offset = 0;
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin("My Window");
  ImGui::SliderFloat("Spread", &sun.blurSpread, 3.0f, 100.0f, NULL ,ImGuiSliderFlags_NoInput);
  ImGui::SliderFloat("Emissive Strength", &sun.emissionStrength, 1.0f, 100.0f, NULL ,ImGuiSliderFlags_NoInput);
  ImGui::SliderFloat("Exposure", &sun.exposure, 0.1f, 5.0f, NULL ,ImGuiSliderFlags_NoInput);
  ImGui::SliderFloat("CameraSpeed", &camera->_speed, 1.0f, 15.0f, NULL ,ImGuiSliderFlags_NoInput);
  ImGui::Checkbox("Togggle Normal Mapping", &earth.toggleNormalMapping);
  ImGui::SliderFloat("Days per second", &daysPerSecond, 0.01f, 10.0f, NULL, ImGuiSliderFlags_NoInput);
  ImGui::Text("Framerate: %f", ImGui::GetIO().Framerate);

  frameTimes[offset] = ImGui::GetIO().DeltaTime * 1000.0f;
  offset = (offset + 1) % 100;
  ImGui::PlotLines("Frame Times (ms)", frameTimes, 100, offset,
                  nullptr, 0.0f, 33.3f, ImVec2(300, 80));
  ImGui::End();
}

void SolarSystem::run() {
  float lastFrameTime = glfwGetTime();
  float deltaTime;
  float currentTime;

  while(!glfwWindowShouldClose(window_)) {
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    processInput(window_, camera);
    // camera->processCameraDirMovement(window_, deltaTime);

    // glm::vec3 cameraPosition = earth.center - (0.15f * earth.center) + glm::vec3(0.0f,5.0f,5.0f); //TODO temporary
    glm::vec3 cameraPosition = earth.center + (5.0f * glm::normalize(glm::cross(earth.center, glm::vec3(0.0f,1.0f,0.0f))));
                                                                                                  
    camera->setPosition(cameraPosition);

    updatePositions(deltaTime); 
    renderer.render(sun, earth, moon, stars, camera);
    drawGUI();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }
}

