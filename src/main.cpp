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

  std::string fragmentShaderSource;
  loadShaderSource("./shaders/fragment.glsl", fragmentShaderSource);
  const GLchar* fragmentShaderSourcePtr = fragmentShaderSource.c_str();

  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderId, 1, &fragmentShaderSourcePtr, nullptr);
  glCompileShader(fragmentShaderId);

  glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShaderId, infoLog.size(), nullptr, infoLog.data());
    std::cout << "ERROR::SHADER::fragment::COMPILATION_FAILED\n" << infoLog.data() << std::endl;
  }

  GLuint shaderProgramId = glCreateProgram();
  glAttachShader(shaderProgramId, vertexShaderId);
  glAttachShader(shaderProgramId, fragmentShaderId);
  glLinkProgram(shaderProgramId);

  glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgramId, infoLog.size(), nullptr, infoLog.data());
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << std::endl;
  }

  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);

  std::array vertices{ 0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f };
  std::array indices{ 0U, 1U, 3U, 1U, 2U, 3U };

  GLuint vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // Render Commands Start

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramId);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Render Commands End

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteProgram(shaderProgramId);

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
