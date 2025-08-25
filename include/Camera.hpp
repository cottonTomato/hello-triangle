#ifndef INCLUDE_INCLUDE_CAMERA_HPP_
#define INCLUDE_INCLUDE_CAMERA_HPP_

#include <glm/glm.hpp>

class Camera
{
 private:
  friend class CameraBuilder;

  glm::vec3 worldUp;
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;

  float yaw;
  float pitch;
  float speed;
  float sensitivity;

  Camera(
      glm::vec3 position,
      glm::vec3 worldUp,
      float yaw,
      float pitch,
      float speed,
      float sensitivity);

 public:
  static constexpr float MAX_PITCH = 89.0F;
  static constexpr float MIN_PITCH = -89.0F;

  enum MovementDirection
  {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
  };

  glm::mat4 getViewMatrix() const;
  void updatePosition(MovementDirection direction, float deltaTime) noexcept;
  void updateDirection(float xoffset, float yoffset) noexcept;

 private:
  void _updateDirection();
};

class CameraBuilder
{
 private:
  glm::vec3 position = DEFAULT_POSITION;
  glm::vec3 worldUp = DEFAULT_WORLD_UP;

  float yaw = DEFAULT_YAW;
  float pitch = DEFAULT_PITCH;
  float speed = DEFAULT_SPEED;
  float sensitivity = DEFAULT_SENSITIVITY;

 public:
  static constexpr glm::vec3 DEFAULT_POSITION = glm::vec3(0.0F, 0.0F, 0.0F);
  static constexpr glm::vec3 DEFAULT_WORLD_UP = glm::vec3(0.0F, 1.0F, 0.0F);

  static constexpr float DEFAULT_YAW = -90.0F;
  static constexpr float DEFAULT_PITCH = 0.0F;
  static constexpr float DEFAULT_SPEED = 2.5F;
  static constexpr float DEFAULT_SENSITIVITY = 0.1F;

  CameraBuilder& setPosition(glm::vec3 position) noexcept;
  CameraBuilder& setPosition(float x, float y, float z) noexcept;
  CameraBuilder& setWorldUp(glm::vec3 worldUp) noexcept;
  CameraBuilder& setWorldUp(float x, float y, float z) noexcept;
  CameraBuilder& withYaw(float yaw) noexcept;
  CameraBuilder& withPitch(float pitch) noexcept;
  CameraBuilder& withSpeed(float speed) noexcept;
  CameraBuilder& withSensitivity(float sensitivity) noexcept;
  Camera build() const;
};

#endif  // INCLUDE_INCLUDE_CAMERA_HPP_
