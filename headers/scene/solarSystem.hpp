#pragma once 
#include "celestialBody.hpp"
#include "camera.hpp"
#include "../../headers/core/application.hpp"
#include "../../headers/rendering/renderer.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <memory>

class SolarSystem {
  private:
    Sun sun;
    Earth earth;
    Moon moon;
    Stars stars;

    GLuint sphereVAO, sphereVBO, sphereEBO;
    GLuint skyboxVAO, skyboxVBO;

    std::unique_ptr<Camera> camera;

    GLFWwindow* window_;
    Renderer renderer;

    float daysPerSecond;

    void updateSpeeds();
    void updatePositions(float deltaTime);
    void updateSunPosition(float deltaTime);
    void updateEarthPosition(float deltaTime);
    void updateMoonPosition(float deltaTime);

  public:
    SolarSystem();
    void drawGUI();
    void run();

};

