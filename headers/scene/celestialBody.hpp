#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

struct Sun {
    GLuint    vao = 0;
    unsigned int idxCount = 0;
    glm::vec3 center;
    float     radius;
    float     spinSpeed;
    float     spinAngle;
    float     blurSpread;
    float     emissionStrength;
    float     exposure;
    GLuint    diffuseTexture = 0;

    Sun(GLuint vao, unsigned int idxCount);
    Sun() = default;

    glm::mat4 getModelMatrix();
};

struct Earth {
    GLuint    vao = 0;
    unsigned int idxCount = 0;
    glm::vec3 center;
    float     radius;
    float     orbitRadius;
    float     spinSpeed;
    float     spinAngle;
    float     orbitAngle;
    float     orbitSpeed;
    float     cloudSpeed;
    float     cloudAngle;
    float     tilt;
    GLuint    diffuseTextureDay = 0;
    GLuint    diffuseTextureNight = 0;
    GLuint    specularMap = 0;
    GLuint    diffuseTextureClouds = 0;

    bool toggleNormalMapping;

    Earth(GLuint vao, unsigned int idxCount);
    Earth() = default;

    glm::mat4 getModelMatrix();
};

struct Moon {
    GLuint    vao = 0;
    unsigned int idxCount = 0;
    glm::vec3 center = glm::vec3(0.0f,0.0f,80.0f);
    float     radius = 0.2f;
    float     orbitRadius = 1.0f;
    float     orbitAngle = 0.0f;
    float     spinSpeed = 1.0f;
    float     orbitSpeed = 1.0f;
    GLuint    diffuseTexture = 0;

    Moon(GLuint vao, unsigned int idxCount);
    Moon() = default;

    glm::mat4 getModelMatrix(Earth& earth);
};

struct Stars {
    GLuint vao = 0;;
    GLuint skybox = 0;

    Stars(GLuint vao);
    Stars() = default;
};
