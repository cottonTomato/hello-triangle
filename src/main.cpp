#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/glad.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

void errorCallback(int error, const char* description);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

int loadShaderSource(const std::string& path, std::string& out);

int main()
{
  glfwSetErrorCallback(errorCallback);

  if (!glfwInit())
  {
    std::cerr << "Failed to init GLFW." << std::endl;

    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hello GLFW", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cerr << "Failed to create GLFW window." << std::endl;
    glfwTerminate();

    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;

    return EXIT_FAILURE;
  }

  std::string vertexShaderSource;
  loadShaderSource("./shaders/vertex.glsl", vertexShaderSource);
  const GLchar* vertexShaderSourcePtr = vertexShaderSource.c_str();

  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShaderId, 1, &vertexShaderSourcePtr, nullptr);
  glCompileShader(vertexShaderId);

  GLint success;
  std::array<char, 512> infoLog;

  glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShaderId, infoLog.size(), nullptr, infoLog.data());
    std::cout << "ERROR::SHADER::vertex::COMPILATION_FAILED\n" << infoLog.data() << std::endl;
  }

  std::string fragment1ShaderSource;
  loadShaderSource("./shaders/fragment1.glsl", fragment1ShaderSource);
  const GLchar* fragment1ShaderSourcePtr = fragment1ShaderSource.c_str();

  GLuint fragment1ShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment1ShaderId, 1, &fragment1ShaderSourcePtr, nullptr);
  glCompileShader(fragment1ShaderId);

  glGetShaderiv(fragment1ShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment1ShaderId, infoLog.size(), nullptr, infoLog.data());
    std::cout << "ERROR::SHADER::fragment1::COMPILATION_FAILED\n" << infoLog.data() << std::endl;
  }

  std::string fragment2ShaderSource;
  loadShaderSource("./shaders/fragment2.glsl", fragment2ShaderSource);
  const GLchar* fragment2ShaderSourcePtr = fragment2ShaderSource.c_str();

  GLuint fragment2ShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment2ShaderId, 1, &fragment2ShaderSourcePtr, nullptr);
  glCompileShader(fragment2ShaderId);

  glGetShaderiv(fragment2ShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment2ShaderId, infoLog.size(), nullptr, infoLog.data());
    std::cout << "ERROR::SHADER::fragment2::COMPILATION_FAILED\n" << infoLog.data() << std::endl;
  }

  GLuint shaderProgram1Id = glCreateProgram();
  glAttachShader(shaderProgram1Id, vertexShaderId);
  glAttachShader(shaderProgram1Id, fragment1ShaderId);
  glLinkProgram(shaderProgram1Id);

  glGetProgramiv(shaderProgram1Id, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram1Id, infoLog.size(), nullptr, infoLog.data());
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << std::endl;
  }

  GLuint shaderProgram2Id = glCreateProgram();
  glAttachShader(shaderProgram2Id, vertexShaderId);
  glAttachShader(shaderProgram2Id, fragment2ShaderId);
  glLinkProgram(shaderProgram2Id);

  glGetProgramiv(shaderProgram2Id, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram2Id, infoLog.size(), nullptr, infoLog.data());
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << std::endl;
  }

  glDeleteShader(vertexShaderId);
  glDeleteShader(fragment1ShaderId);
  glDeleteShader(fragment2ShaderId);

  std::array vertices1{
    -0.5F, -0.5F, 0.0F, -0.25F, 0.5F, 0.0F, 0.0F, -0.5F, 0.0F,
  };
  std::array vertices2{ 0.0F, -0.5F, 0.0F, 0.25F, 0.5F, 0.0F, 0.5F, -0.5F, 0.0F };

  GLuint vao1, vbo1;
  glGenVertexArrays(1, &vao1);
  glGenBuffers(1, &vbo1);

  glBindVertexArray(vao1);

  glBindBuffer(GL_ARRAY_BUFFER, vbo1);
  glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(float), vertices1.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  GLuint vao2, vbo2;
  glGenVertexArrays(1, &vao2);
  glGenBuffers(1, &vbo2);

  glBindVertexArray(vao2);

  glBindBuffer(GL_ARRAY_BUFFER, vbo2);
  glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(float), vertices2.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // Render Commands Start

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram1Id);
    glBindVertexArray(vao1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram2Id);
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Render Commands End

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao1);
  glDeleteBuffers(1, &vbo1);
  glDeleteBuffers(1, &vao2);
  glDeleteBuffers(1, &vbo2);
  glDeleteProgram(shaderProgram1Id);
  glDeleteProgram(shaderProgram2Id);

  glfwTerminate();

  return EXIT_SUCCESS;
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

int loadShaderSource(const std::string& path, std::string& out)
{
  std::ifstream file(path);

  if (!file.is_open())
    return -1;

  std::stringstream ss;
  ss << file.rdbuf();
  out = ss.str();

  return 0;
}

void errorCallback(int error, const char* description)
{
  std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void frameBufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
