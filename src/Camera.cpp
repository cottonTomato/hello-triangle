#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

Camera::Camera(
    glm::vec3 position,
    glm::vec3 worldUp,
    float yaw,
    float pitch,
    float speed,
    float sensitivity)
    : worldUp(worldUp),
      position(position),
      yaw(yaw),
      pitch(pitch),
      speed(speed),
      sensitivity(sensitivity)
{
  _updateDirection();
}

glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(position, position + front, up);
}

void Camera::updatePosition(Camera::MoveDir direction, float deltaTime) noexcept
{
  float velocity = speed * deltaTime;
  switch (direction)
  {
    case MoveDir::FORWARD: position += front * velocity; break;
    case MoveDir::BACKWARD: position -= front * velocity; break;
    case MoveDir::LEFT: position -= right * velocity; break;
    case MoveDir::RIGHT: position += right * velocity; break;
  }
}

void Camera::updateDirection(float xoffset, float yoffset)
{
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  pitch = glm::clamp(pitch, MIN_PITCH, MAX_PITCH);

  _updateDirection();
}

glm::vec3 Camera::getPosition() const noexcept
{
  return position;
}

glm::vec3 Camera::getFront() const noexcept
{
  return front;
}

glm::vec3 Camera::getUp() const noexcept
{
  return up;
}

glm::vec3 Camera::getWorldUp() const noexcept
{
  return worldUp;
}

glm::vec3 Camera::getRight() const noexcept
{
  return right;
}

void Camera::_updateDirection()
{
  glm::vec3 newFront(
      glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)),
      glm::sin(glm::radians(pitch)),
      glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw)));

  front = glm::normalize(newFront);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

CameraBuilder& CameraBuilder::setPosition(glm::vec3 position) noexcept
{
  this->position = position;
  return *this;
}

CameraBuilder& CameraBuilder::setPosition(float x, float y, float z) noexcept
{
  this->position = glm::vec3(x, y, z);
  return *this;
}

CameraBuilder& CameraBuilder::setWorldUp(glm::vec3 worldUp) noexcept
{
  this->worldUp = worldUp;
  return *this;
}

CameraBuilder& CameraBuilder::setWorldUp(float x, float y, float z) noexcept
{
  this->worldUp = glm::vec3(x, y, z);
  return *this;
}

CameraBuilder& CameraBuilder::withPitch(float pitch) noexcept
{
  this->pitch = pitch;
  return *this;
}

CameraBuilder& CameraBuilder::withYaw(float yaw) noexcept
{
  this->yaw = yaw;
  return *this;
}

CameraBuilder& CameraBuilder::withSpeed(float speed) noexcept
{
  this->speed = speed;
  return *this;
}

CameraBuilder& CameraBuilder::withSensitivity(float sensitivity) noexcept
{
  this->sensitivity = sensitivity;
  return *this;
}

Camera CameraBuilder::build() const
{
  return Camera(position, worldUp, yaw, pitch, speed, sensitivity);
}
