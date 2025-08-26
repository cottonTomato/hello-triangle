#include "Projection.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

Projection::Projection(
    float fov,
    float minFov,
    float maxFov,
    float aspectRatio,
    float near,
    float far) noexcept
    : minFov(minFov),
      maxFov(maxFov),
      aspectRatio(aspectRatio),
      near(near),
      far(far)
{
  setFov(fov);
}

void Projection::setFovRange(float min, float max) noexcept
{
  minFov = min;
  maxFov = max;
  setFov(fov);
}

void Projection::setFov(float fov) noexcept
{
  this->fov = glm::clamp(fov, minFov, maxFov);
}

void Projection::updateFov(float yoffset) noexcept
{
  fov -= yoffset;
  setFov(fov);
}

glm::mat4 Projection::getProjectionMatrix() const
{
  glm::mat4 mat = glm::perspective(glm::radians(fov), aspectRatio, near, far);
  return mat;
}

ProjectionBuilder& ProjectionBuilder::withFov(float fov) noexcept
{
  this->fov = fov;
  return *this;
}

ProjectionBuilder& ProjectionBuilder::withFovRange(
    float min,
    float max) noexcept
{
  minFov = min;
  maxFov = max;
  return *this;
}

ProjectionBuilder& ProjectionBuilder::withAspectRatio(
    float aspectRatio) noexcept
{
  this->aspectRatio = aspectRatio;
  return *this;
}

ProjectionBuilder& ProjectionBuilder::withZRange(float near, float far) noexcept
{
  this->near = near;
  this->far = far;
  return *this;
}

Projection ProjectionBuilder::build() const
{
  if (minFov >= maxFov)
  {
    throw std::runtime_error("Invalid Argument: Fov Range");
  }
  if (near >= far)
  {
    throw std::runtime_error("Invalid Argument: Z Range");
  }
  if (aspectRatio < 0.0F)
  {
    throw std::runtime_error("Invalid Argument: Aspect Ratio");
  }

  return Projection(fov, minFov, maxFov, aspectRatio, near, far);
}
