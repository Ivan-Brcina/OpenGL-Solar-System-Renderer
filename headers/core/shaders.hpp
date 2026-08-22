#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

class Shader {
private:
  GLuint _program;

  GLuint CompileShader(const std::string &fileSource, GLint type);
  GLuint checkUniLocation(const std::string& name);

public:
  Shader() = default;
  //Izmijeni kurati default initalizer TODO
  Shader(const std::string& vertexShSource, 
         const std::string& fragmentShSource, 
         const std::string& tessellationShSource = "");

  void setUniformMat4(const std::string &name, glm::mat4 matrix);
  void setUniformMat3(const std::string &name, glm::mat3 matrix);
  void setUniformVec3(const std::string &name, glm::vec3 vector);
  void setUniformi(const std::string &name, int num);
  void setUniformf(const std::string& name, float num);
  void setUniformMaterial(const Material& material);
  GLuint getProgram() {return _program;}
  void use();
};


