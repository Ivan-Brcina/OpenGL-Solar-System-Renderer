#include "../../headers/scene/camera.hpp"
#include <GLFW/glfw3.h>

#include <iostream>

Camera* Camera::currCam = nullptr;

// Camera::Camera() 
//   :
//     up_(0.0f, 1.0f, 0.0f),
//     radius_(7.0f),
//     previousX_(0.0f),
//     previousY_(0.0f),
//     firstClick_(true)
// {
//   position_ = glm::vec3(0.0f, 5.0f, radius_);
// }
//
// glm::mat4 Camera::getView() const {
//   return glm::lookAt(position_, glm::vec3(0.0f,0.0f,0.0f), up_);
// }
//
// glm::mat4 Camera::getProjection(float vw, float vh) const {
//   return glm::perspective(glm::radians(60.0f), vw / vh, 0.1f, 100.0f);
// }
//
// void Camera::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
//
//   if(!currCam) return;
//
//   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
//
//     currCam->firstClick_ = true;
//      return;
//
//   } else {
//
//     float sensitivity = 0.1f;
//
//     if(currCam->firstClick_) {
//       currCam->previousX_ = xPos;
//       currCam->previousY_ = yPos;
//       currCam->firstClick_ = false;
//       return;
//     }
//
//     float xOffset = (xPos - currCam->previousX_) * sensitivity;
//     float yOffset = (currCam->previousY_ - yPos) * sensitivity;
//
//     currCam->previousX_ = xPos;
//     currCam->previousY_ = yPos;
//
//     glm::vec3 rightAxis = glm::normalize(glm::cross(currCam->position_, currCam->up_));
//     glm::quat q1 = glm::angleAxis(glm::radians(yOffset), rightAxis);
//
//     glm::vec3 rotationAxis = currCam->up_;
//     glm::quat q2 = glm::angleAxis(glm::radians(xOffset), rotationAxis);
//
//     currCam->position_ = q1 * q2 * currCam->position_;
//     currCam->up_ = q1 * q2 * currCam->up_;
//   }
//
// }

Camera::Camera() 
  : _position(0.0f, 20.0f, 0.0f),
    _front(0.0f, 0.0f, -1.0f),
    _up(0.0f, 1.0f, 0.0f),
    _yaw(-90.0f),
    _pitch(0.0f),
    _lastX(960.0f / 2.0f),
    _lastY(720.0f / 2.0f),
    _sensitivity(0.1f),
    _speed(15.0f),
    _firstMouse(true)
{}

void Camera::activate() {
  currCam = this;
}

glm::mat4 Camera::getView() const {
  return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4 Camera::getProjection(float vw, float vh) const {
  return glm::perspective(glm::radians(60.0f), vw / vh, 0.1f, 200.0f);
}

glm::vec3 Camera::getPosition() const {return _position;}

void Camera::setPosition(glm::vec3& newPosition) {
  _position = newPosition;
}

void Camera::setView(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up) {
  _position = pos;
  _front = front;
  _up = up;
}

void Camera::processCameraDirMovement(GLFWwindow* window, float deltaTime) {
  const float cameraSpeed = _speed * deltaTime;
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    _position = _position + _front * cameraSpeed;
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    _position = _position - _front * cameraSpeed;
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
    _position = _position - glm::normalize(glm::cross(_front, _up)) * cameraSpeed;
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
    _position = _position + glm::normalize(glm::cross(_front, _up)) * cameraSpeed;
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
    _position = _position + glm::normalize(_up) * cameraSpeed;
  if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) 
    _position = _position - glm::normalize(_up) * cameraSpeed;
}

void Camera::setFront() {
  if(_pitch > 89.0f) 
    _pitch = 89.0f;
  if(_pitch < -89.0f) 
    _pitch = -89.0f;

  glm::vec3 direction;
  direction.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
  direction.y = glm::sin(glm::radians(_pitch));
  direction.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
  _front = direction;
}

glm::vec3 Camera::getFront() const { return this->_front;}

void Camera::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
  if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
        return;

  if(!currCam) return;
  if(currCam->cursorEnabled) return;
  (void)window;

  if(currCam->_firstMouse == GL_TRUE) {
    currCam->_lastY = yPos;
    currCam->_lastX = xPos;
    currCam->_firstMouse = false;
  }

  float yOffset = currCam->_lastY - yPos;
  float xOffset = xPos - currCam->_lastX;
  currCam->_lastX = xPos;
  currCam->_lastY = yPos;

  xOffset *= currCam->_sensitivity;
  yOffset *= currCam->_sensitivity;
  
  currCam->_yaw += xOffset;
  currCam->_pitch += yOffset;
  currCam->setFront();
}
