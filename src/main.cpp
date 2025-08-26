#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <array>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"
#include "Projection.hpp"
#include "Shader.hpp"
#include "glad/glad.h"
#include "stb/image.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;
constexpr float aspectRatio = static_cast<float>(windowWidth) / windowHeight;

Camera camera = CameraBuilder().setPosition(0.0F, 0.0F, 3.0F).build();
Projection cameraProjection =
    ProjectionBuilder().withAspectRatio(aspectRatio).build();

void errorCallback(int error, const char* description);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;

    return EXIT_FAILURE;
  }

  glEnable(GL_DEPTH_TEST);

  Shader shp("./shaders/vertex.glsl", "./shaders/fragment1.glsl");

  std::array cubePostitons{
    glm::vec3(0.0F, 0.0F, 0.0F),    glm::vec3(2.0F, 5.0F, -15.0F),
    glm::vec3(-1.5F, -2.2F, -2.5F), glm::vec3(-3.8F, -2.0F, -12.3F),
    glm::vec3(2.4F, -0.4F, -3.5F),  glm::vec3(-1.7F, 3.0F, -7.5F),
    glm::vec3(1.3F, -2.0F, -2.5F),  glm::vec3(1.5F, 2.0F, -2.5F),
    glm::vec3(1.5F, 0.2F, -1.5F),   glm::vec3(-1.3F, 1.0F, -1.5F)
  };

  std::array vertices1{
    -0.5F, -0.5F, -0.5F, 0.0F,  0.0F,  0.5F,  -0.5F, -0.5F, 1.0F,  0.0F,  0.5F,
    0.5F,  -0.5F, 1.0F,  1.0F,  0.5F,  0.5F,  -0.5F, 1.0F,  1.0F,  -0.5F, 0.5F,
    -0.5F, 0.0F,  1.0F,  -0.5F, -0.5F, -0.5F, 0.0F,  0.0F,  -0.5F, -0.5F, 0.5F,
    0.0F,  0.0F,  0.5F,  -0.5F, 0.5F,  1.0F,  0.0F,  0.5F,  0.5F,  0.5F,  1.0F,
    1.0F,  0.5F,  0.5F,  0.5F,  1.0F,  1.0F,  -0.5F, 0.5F,  0.5F,  0.0F,  1.0F,
    -0.5F, -0.5F, 0.5F,  0.0F,  0.0F,  -0.5F, 0.5F,  0.5F,  1.0F,  0.0F,  -0.5F,
    0.5F,  -0.5F, 1.0F,  1.0F,  -0.5F, -0.5F, -0.5F, 0.0F,  1.0F,  -0.5F, -0.5F,
    -0.5F, 0.0F,  1.0F,  -0.5F, -0.5F, 0.5F,  0.0F,  0.0F,  -0.5F, 0.5F,  0.5F,
    1.0F,  0.0F,  0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.5F,  0.5F,  -0.5F, 1.0F,
    1.0F,  0.5F,  -0.5F, -0.5F, 0.0F,  1.0F,  0.5F,  -0.5F, -0.5F, 0.0F,  1.0F,
    0.5F,  -0.5F, 0.5F,  0.0F,  0.0F,  0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  -0.5F,
    -0.5F, -0.5F, 0.0F,  1.0F,  0.5F,  -0.5F, -0.5F, 1.0F,  1.0F,  0.5F,  -0.5F,
    0.5F,  1.0F,  0.0F,  0.5F,  -0.5F, 0.5F,  1.0F,  0.0F,  -0.5F, -0.5F, 0.5F,
    0.0F,  0.0F,  -0.5F, -0.5F, -0.5F, 0.0F,  1.0F,  -0.5F, 0.5F,  -0.5F, 0.0F,
    1.0F,  0.5F,  0.5F,  -0.5F, 1.0F,  1.0F,  0.5F,  0.5F,  0.5F,  1.0F,  0.0F,
    0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  -0.5F, 0.5F,  0.5F,  0.0F,  0.0F,  -0.5F,
    0.5F,  -0.5F, 0.0F,  1.0F
  };

  GLuint vao1, vbo1;
  glGenVertexArrays(1, &vao1);
  glGenBuffers(1, &vbo1);

  glBindVertexArray(vao1);

  glBindBuffer(GL_ARRAY_BUFFER, vbo1);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(vertices1), vertices1.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      5 * sizeof(decltype(vertices1)::value_type),
      reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1,
      2,
      GL_FLOAT,
      GL_FALSE,
      5 * sizeof(decltype(vertices1)::value_type),
      reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  stbi_set_flip_vertically_on_load(true);

  int height, width, nrChannels;
  unsigned char* data = stbi_load(
      "assets/textures/container.jpg", &width, &height, &nrChannels, 0);

  GLuint textureId1;
  glGenTextures(1, &textureId1);

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

  glBindTexture(GL_TEXTURE_2D, 0);

  shp.bind();
  shp.setInt("texture1", 0);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // Render Commands Start

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId1);

    shp.bind();

    float t = static_cast<float>(glfwGetTime());

    glm::mat4 view = camera.getViewMatrix();
    shp.setMatrix("view", view);

    glm::mat4 projection = cameraProjection.getProjectionMatrix();
    shp.setMatrix("projection", projection);

    glBindVertexArray(vao1);
    for (std::size_t i = 0; i < cubePostitons.size(); i++)
    {
      glm::mat4 model(1.0F);
      model = glm::translate(model, cubePostitons[i]);
      if (i % 3 == 0)
      {
        model = glm::rotate(
            model, glm::radians(32.0F * t), glm::vec3(0.5F, 1.0F, 0.0F));
      }
      model = glm::rotate(
          model, glm::radians(20.0F * i), glm::vec3(1.0f, 0.3f, 0.5f));

      shp.setMatrix("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
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

  static float lastFrame = 0.0F;
  float currentFrame = static_cast<float>(glfwGetTime());
  float deltaFrame = currentFrame - lastFrame;
  lastFrame = currentFrame;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.updatePosition(Camera::FORWARD, deltaFrame);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.updatePosition(Camera::BACKWARD, deltaFrame);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.updatePosition(Camera::LEFT, deltaFrame);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.updatePosition(Camera::RIGHT, deltaFrame);
}

void mouseCallback(
    [[maybe_unused]] GLFWwindow* window,
    double xpos,
    double ypos)
{
  float xposF = static_cast<float>(xpos), yposF = static_cast<float>(ypos);

  static bool firstMouse = true;
  static float lastX, lastY;
  if (firstMouse)
  {
    lastX = xposF;
    lastY = yposF;
    firstMouse = false;
  }

  float xoffset = xposF - lastX, yoffset = lastY - yposF;
  lastX = xposF, lastY = yposF;

  camera.updateDirection(xoffset, yoffset);
}

void scrollCallback(
    [[maybe_unused]] GLFWwindow* window,
    [[maybe_unused]] double xoffset,
    double yoffset)
{
  cameraProjection.updateFov(static_cast<float>(yoffset));
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
