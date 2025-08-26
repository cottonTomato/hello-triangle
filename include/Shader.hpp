#ifndef INCLUDE_INCLUDE_SHADER_HPP_
#define INCLUDE_INCLUDE_SHADER_HPP_

#include <glm/glm.hpp>
#include <string>

class Shader
{
 private:
  unsigned int programId;

 public:
  Shader(const std::string& vertexPath, const std::string& fragmentPath);

  ~Shader() noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;

  unsigned int getProgramId() const noexcept;

  void setInt(const std::string& name, int value) const noexcept;
  void setFloat(const std::string& name, float value) const noexcept;
  void setBool(const std::string& name, bool value) const noexcept;
  void setVec3(const std::string& name, glm::vec3 vec) const noexcept;
  void setVec3(const std::string& name, float x, float y, float z)
      const noexcept;
  void setMat3(const std::string& name, glm::mat3 mat) const noexcept;
  void setMat4(const std::string& name, glm::mat4 mat) const noexcept;

 private:
  void checkStatus(unsigned int id, const std::string& type) const;
};

#endif  // INCLUDE_INCLUDE_SHADER_HPP_
