#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <memory>
#include "../core/shaders.hpp"
#include "../scene/celestialBody.hpp"
#include "../scene/camera.hpp"

class ScenePass {
  GLFWwindow* window;
  std::unique_ptr<Shader> hdrShader, skyboxShader, earthShader, moonShader, cloudsShader;
  GLuint FBO, RBO;
  GLuint colorBuffers[2];

  private:
    void setupHdrFramebuffer();
    
    void renderSun(Sun& sun, const glm::mat4& view, const glm::mat4& projection,
                   std::unique_ptr<Camera>& camera);

    void renderEarth(Earth& earth, const glm::mat4& view, const glm::mat4& projection, GLuint depthMap,
                     std::unique_ptr<Camera>& camera, const glm::mat4& lightSpaceMatrix);

    void renderClouds(Earth& earth, const glm::mat4& view, const glm::mat4& projection, GLuint depthMap,
                      const glm::mat4& lightSpaceMatrix);

    void renderMoon(Earth& earth, Moon& moon, const glm::mat4& view, const glm::mat4& projection, GLuint depthMap,
                    const glm::mat4& lightSpaceMatrix);

    void renderSkyBox(Stars& stars, const glm::mat4& view, const glm::mat4& projection);
  public:
    ScenePass(GLFWwindow* window);
    ScenePass() = default;
    void render(Sun& sun, Earth& earth, Moon& moon, Stars& stars, 
                std::unique_ptr<Camera>& camera, GLuint depthMap,
                const glm::mat4& lightSpaceMatrix);
  public:
    GLuint getHDRColorBuffer() const {return colorBuffers[0];}
    GLuint getBrightColorBuffer() const {return colorBuffers[1];}

};

