#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/shaders.hpp"
#include "../scene/celestialBody.hpp"
#include <memory>

class ShadowPass {
  private:
    GLFWwindow* window_;
    std::unique_ptr<Shader> depthShader_;
    GLuint depthMap_ = 0;
    GLuint depthFBO = 0;
    GLuint depthMapWidth;
    GLuint depthMapHeight;
    float frustumDimension;
    glm::mat4 lightSpaceTransform;

    void setupDepthBuffer();
    void renderToDepthMap(Earth& earth, Moon& moon);
    void renderEarth(Earth& earth);
    void renderMoon(Earth& earth, Moon& moon);

  public:
    ShadowPass(GLFWwindow* window);
    ShadowPass() = default;

    void render(Earth& earth, Moon& moon);

    GLuint getDepthMap() const {return depthMap_;}
    glm::mat4 getLightSpaceTransform() const {return lightSpaceTransform;}
};
