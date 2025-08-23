#ifndef INCLUDE_INCLUDE_SHADER_HPP_
#define INCLUDE_INCLUDE_SHADER_HPP_

#include <string>

class Shader
{
 private:
  unsigned int programId;

 public:
  Shader(const std::string& vertexPath, const std::string& fragmentPath);

  void use() const;

  unsigned int getProgramId() const;

  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setBool(const std::string& name, bool value) const;

  ~Shader();

 private:
  void checkStatus(unsigned int id, const std::string& type);
};

#endif  // INCLUDE_INCLUDE_SHADER_HPP_
