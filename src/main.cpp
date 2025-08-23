#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <array>
#include <cstdlib>
#include <iostream>

#include "Shader.hpp"
#include "glad/glad.h"
#include "stb/image.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

void errorCallback(int error, const char* description);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

int loadShaderSource(const std::string& path, std::string& out);

int mixRatio = 0;

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

  GLFWwindow* window = glfwCreateWindow(
      windowWidth, windowHeight, "Hello GLFW", nullptr, nullptr);
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

  Shader shp("./shaders/vertex.glsl", "./shaders/fragment1.glsl");

  std::array vertices1{
    // positions                        // colors                     // texture
    0.5F,  0.5F,  0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1.0F,  // top right
    0.5F,  -0.5F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0.0F,  // bottom right
    -0.5F, -0.5F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,  // bottom left
    -0.5F, 0.5F,  0.0F, 1.0F, 1.0F, 0.0F, 0.0F, 1.0F   // top left
  };
  std::array indices{ 0U, 1U, 3U, 1U, 2U, 3U };

  GLuint vao1, vbo1, ebo1;
  glGenVertexArrays(1, &vao1);
  glGenBuffers(1, &vbo1);
  glGenBuffers(1, &ebo1);

  glBindVertexArray(vao1);

  glBindBuffer(GL_ARRAY_BUFFER, vbo1);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(vertices1), vertices1.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      8 * sizeof(decltype(vertices1)::value_type),
      reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1,
      3,
      GL_FLOAT,
      GL_FALSE,
      8 * sizeof(decltype(vertices1)::value_type),
      reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2,
      2,
      GL_FLOAT,
      GL_FALSE,
      8 * sizeof(decltype(vertices1)::value_type),
      reinterpret_cast<void*>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  stbi_set_flip_vertically_on_load(true);

  int height, width, nrChannels;
  unsigned char* data = stbi_load(
      "assets/textures/container.jpg", &width, &height, &nrChannels, 0);

  GLuint textureId1, textureId2;
  glGenTextures(1, &textureId1);
  glGenTextures(1, &textureId2);

  glBindTexture(GL_TEXTURE_2D, textureId1);
  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      width,
      height,
      0,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  data = stbi_load(
      "assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);

  glBindTexture(GL_TEXTURE_2D, textureId2);
  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      width,
      height,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  glBindTexture(GL_TEXTURE_2D, 0);

  shp.use();
  shp.setInt("texture1", 0);
  shp.setInt("texture2", 1);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // Render Commands Start

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    shp.setFloat("mixRatio", mixRatio / 10.0F);

    shp.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId2);

    // glActiveTexture(textureId);
    glBindVertexArray(vao1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Render Commands End

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao1);
  glDeleteBuffers(1, &vbo1);

  glfwTerminate();

  return EXIT_SUCCESS;
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  static int prevUpState = GLFW_RELEASE;
  static int prevDownState = GLFW_RELEASE;

  int upState = glfwGetKey(window, GLFW_KEY_UP);
  int downState = glfwGetKey(window, GLFW_KEY_DOWN);

  if (upState == GLFW_PRESS && prevUpState == GLFW_RELEASE)
  {
    if (mixRatio < 10)
    {
      mixRatio += 1;
    }
  }

  if (downState == GLFW_PRESS && prevDownState == GLFW_RELEASE)
  {
    if (mixRatio > 0)
    {
      mixRatio -= 1;
    }
  }

  prevUpState = upState;
  prevDownState = downState;
}

void errorCallback(int error, const char* description)
{
  std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void frameBufferSizeCallback(
    [[maybe_unused]] GLFWwindow* window,
    int width,
    int height)
{
  glViewport(0, 0, width, height);
}
