#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
private:
  static Camera *currCam;

  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _up;

  float _yaw;
  float _pitch;

  float _lastX;
  float _lastY;
  float _sensitivity;
  

  void setFront();

public:
  float _speed;
  bool _firstMouse;
  bool cursorEnabled = false;
  Camera();
  glm::mat4 getView() const;
  glm::mat4 getProjection(float vw, float vh) const;
  glm::vec3 getPosition() const;
  glm::vec3 getFront() const;

  void setPosition(glm::vec3& newPosition);

  void activate();

  void processCameraDirMovement(GLFWwindow *window, float deltaTime);
  static void mouseCallback(GLFWwindow *window, double xPos, double yPos);

  void setView(const glm::vec3 &pos, const glm::vec3 &front, const glm::vec3 &up);
};
