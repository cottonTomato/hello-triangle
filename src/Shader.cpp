#include "Shader.hpp"

#include <array>
#include <fstream>
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

GLuint Shader::getProgramId() const
{
  return programId;
}

void Shader::use() const
{
  glUseProgram(programId);
}

void Shader::setInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

Shader::~Shader()
{
  glDeleteProgram(programId);
}

void Shader::checkStatus(GLuint id, const std::string& type)
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
