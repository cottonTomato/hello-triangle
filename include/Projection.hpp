#ifndef INCLUDE_INCLUDE_PROJECTION_HPP_
#define INCLUDE_INCLUDE_PROJECTION_HPP_

#include <glm/glm.hpp>

class Projection
{
 private:
  friend class ProjectionBuilder;

  float minFov;
  float maxFov;
  float fov;

  float aspectRatio;

  float near;
  float far;

  Projection(
      float fov,
      float minFov,
      float maxFov,
      float aspectRatio,
      float near,
      float far) noexcept;

 public:
  void setFovRange(float min, float max) noexcept;
  void setFov(float fov) noexcept;
  void updateFov(float yoffset) noexcept;

  glm::mat4 getProjectionMatrix() const;
};

class ProjectionBuilder
{
 private:
  float fov = DEFAULT_FOV;
  float minFov = DEFAULT_MIN_FOV;
  float maxFov = DEFAULT_MAX_FOV;
  float near = DEFAULT_NEAR;
  float far = DEFAULT_FAR;
  float aspectRatio = -1.0F;

 public:
  static constexpr float DEFAULT_FOV = 45.0F;
  static constexpr float DEFAULT_MIN_FOV = 1.0F;
  static constexpr float DEFAULT_MAX_FOV = 60.0F;

  static constexpr float DEFAULT_NEAR = 0.1F;
  static constexpr float DEFAULT_FAR = 100.0F;

  ProjectionBuilder& withFov(float fov) noexcept;
  ProjectionBuilder& withFovRange(float min, float max) noexcept;
  ProjectionBuilder& withAspectRatio(float aspectRatio) noexcept;
  ProjectionBuilder& withZRange(float near, float far) noexcept;

  Projection build() const;
};

#endif  // INCLUDE_INCLUDE_PROJECTION_HPP_
