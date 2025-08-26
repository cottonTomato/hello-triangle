#include "Shader.hpp"

#include <array>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "glad/glad.h"

Shader::Shader(const std::string& vPath, const std::string& fPath)
{
  std::string vSource, fSource;
  std::stringstream vSourceSS, fSourceSS;
  std::ifstream vShaderFile, fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  vShaderFile.open(vPath);
  fShaderFile.open(fPath);

  vSourceSS << vShaderFile.rdbuf();
  fSourceSS << fShaderFile.rdbuf();

  vSource = vSourceSS.str();
  fSource = fSourceSS.str();

  const char *vSourcePtr = vSource.c_str(), *fSourcePtr = fSource.c_str();

  GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexId, 1, &vSourcePtr, nullptr);
  glCompileShader(vertexId);
  checkStatus(vertexId, "VERTEX");

  GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentId, 1, &fSourcePtr, nullptr);
  glCompileShader(fragmentId);
  checkStatus(fragmentId, "FRAGMENT");

  programId = glCreateProgram();
  glAttachShader(programId, vertexId);
  glAttachShader(programId, fragmentId);

  glLinkProgram(programId);
  checkStatus(programId, "PROGRAM");

  glDeleteShader(vertexId);
  glDeleteShader(fragmentId);
}

Shader::~Shader() noexcept
{
  glDeleteProgram(programId);
}

GLuint Shader::getProgramId() const noexcept
{
  return programId;
}

void Shader::bind() const noexcept
{
  glUseProgram(programId);
}

void Shader::unbind() const noexcept
{
  glUseProgram(0);
}

void Shader::setInt(const std::string& name, int value) const noexcept
{
  glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const noexcept
{
  glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value) const noexcept
{
  glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, glm::vec3 vec) const noexcept
{
  glUniform3fv(
      glGetUniformLocation(programId, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
    const noexcept
{
  glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void Shader::setMat3(const std::string& name, glm::mat3 mat) const noexcept
{
  glUniformMatrix3fv(
      glGetUniformLocation(programId, name.c_str()),
      1,
      GL_FALSE,
      glm::value_ptr(mat));
}

void Shader::setMat4(const std::string& name, glm::mat4 mat) const noexcept
{
  glUniformMatrix4fv(
      glGetUniformLocation(programId, name.c_str()),
      1,
      GL_FALSE,
      glm::value_ptr(mat));
}

void Shader::checkStatus(GLuint id, const std::string& type) const
{
  using namespace std::string_literals;

  GLint success;
  std::array<char, 512> infoLog;

  if (type == "PROGRAM")
  {
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(id, infoLog.size(), nullptr, infoLog.data());
      throw std::runtime_error(
          "ERROR::SHADER::PROGRAM::LINKING_FAILED: "s + infoLog.data());
    }
  }
  else
  {
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(id, infoLog.size(), nullptr, infoLog.data());
      throw std::runtime_error(
          "ERROR::SHADER::" + type + "::COMPILATION_FAILED: " + infoLog.data());
    }
  }
}
