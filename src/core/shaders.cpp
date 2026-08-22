#include <GL/glew.h>
#include "../../headers/core/shaders.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

GLuint Shader::CompileShader(const std::string &fileSource, int type) {
  std::ifstream file = std::ifstream(fileSource);

  if (!file.is_open()) {
    std::cout << "Couldn't open shader file!" << std::endl;
  }

  std::string line;
  std::string shaderText;
  
  while (std::getline(file, line)) {
    shaderText += line + "\n";
  }
  
  std::cout << "Shader source length for " << fileSource << ": " << shaderText.length() << std::endl;
  if (shaderText.empty()) {
      std::cout << "EMPTY SHADER SOURCE — file read failed or empty!" << std::endl;
  }

  const char *shaderTextC = shaderText.c_str();
  GLuint shaderObject = glCreateShader(type);
  glShaderSource(shaderObject, 1, &shaderTextC, 0);
  glCompileShader(shaderObject);

  int compiled;
  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compiled);

  if (compiled != GL_TRUE) {
    GLsizei log_length = 0;
    GLchar message[1024];
    glGetShaderInfoLog(shaderObject, 1024, &log_length, message);
    switch(type) {
      case GL_VERTEX_SHADER:
        std::cout << "Vertex";
        break;
      case GL_FRAGMENT_SHADER:
        std::cout << "Fragment";
        break;
      case GL_TESS_EVALUATION_SHADER:
        std::cout << "Tessellation evaluation";
        break;
    }
    std::cout << " Shader compile error: ";
    std::cout << message << std::endl;
  }

  return shaderObject;
}

Shader::Shader(const std::string& vertexShSource,
               const std::string& fragmentShSource, 
               const std::string& tessellationShSource) {

  GLuint vertexShader = CompileShader(vertexShSource, GL_VERTEX_SHADER);
  GLuint fragmentShader = CompileShader(fragmentShSource, GL_FRAGMENT_SHADER);

  GLuint program;
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  if(!tessellationShSource.empty()) {
    GLuint tessellationShader = CompileShader(tessellationShSource, GL_TESS_EVALUATION_SHADER);
    glAttachShader(program, tessellationShader);
    glDeleteShader(tessellationShader);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glBindFragDataLocation(program, 0, "outColor");
  glBindFragDataLocation(program, 1, "brightColor");
  glLinkProgram(program); 

  GLint success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
      GLchar infoLog[1024];
      glGetProgramInfoLog(program, 1024, NULL, infoLog);
      std::cout << "LINK FAILED:\n" << infoLog << std::endl;
  } else {
      std::cout << "Program linked OK" << std::endl;
  }

  glUseProgram(program);
  this->_program = program;
}

void Shader::use() {glUseProgram(_program);}

GLuint Shader::checkUniLocation(const std::string& name) {
  GLint uniLocation = glGetUniformLocation(_program, name.c_str());
  if(uniLocation < 0) {
      std::cout<<"The uniform location '" << name << "' is invalid" << std::endl;
      throw std::runtime_error("Uniform location " + name + " is invalid\n");
    }
  return uniLocation;
}

void Shader::setUniformMat4(const std::string& name, glm::mat4 matrix) {
  glUniformMatrix4fv(checkUniLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformMat3(const std::string& name, glm::mat3 matrix) {
  glUniformMatrix3fv(checkUniLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformVec3(const std::string& name, glm::vec3 vector) {
 glUniform3fv(checkUniLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniformi(const std::string& name, int num) {
  glUniform1i(checkUniLocation(name), num);
}

void Shader::setUniformf(const std::string& name, float num) {
  glUniform1f(checkUniLocation(name), num);
}

void Shader::setUniformMaterial(const Material& material) {
  setUniformVec3("material.ambient", material.ambient);
  setUniformVec3("material.diffuse", material.diffuse);
  setUniformVec3("material.specular", material.specular);
  setUniformf("material.shininess", material.shininess * 128.0f);
}

