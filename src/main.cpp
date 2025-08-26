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

  Shader worldShader("./shaders/vertex1.glsl", "./shaders/fragment1.glsl");
  Shader lightShader("./shaders/vertex2.glsl", "./shaders/fragment2.glsl");

  std::array vertices{
    -0.5F, -0.5F, -0.5F, 0.0F,  0.0F,  -1.0F, 0.0F,  0.0F,  0.5F,  -0.5F, -0.5F,
    0.0F,  0.0F,  -1.0F, 1.0F,  0.0F,  0.5F,  0.5F,  -0.5F, 0.0F,  0.0F,  -1.0F,
    1.0F,  1.0F,  0.5F,  0.5F,  -0.5F, 0.0F,  0.0F,  -1.0F, 1.0F,  1.0F,  -0.5F,
    0.5F,  -0.5F, 0.0F,  0.0F,  -1.0F, 0.0F,  1.0F,  -0.5F, -0.5F, -0.5F, 0.0F,
    0.0F,  -1.0F, 0.0F,  0.0F,  -0.5F, -0.5F, 0.5F,  0.0F,  0.0F,  1.0F,  0.0F,
    0.0F,  0.5F,  -0.5F, 0.5F,  0.0F,  0.0F,  1.0F,  1.0F,  0.0F,  0.5F,  0.5F,
    0.5F,  0.0F,  0.0F,  1.0F,  1.0F,  1.0F,  0.5F,  0.5F,  0.5F,  0.0F,  0.0F,
    1.0F,  1.0F,  1.0F,  -0.5F, 0.5F,  0.5F,  0.0F,  0.0F,  1.0F,  0.0F,  1.0F,
    -0.5F, -0.5F, 0.5F,  0.0F,  0.0F,  1.0F,  0.0F,  0.0F,  -0.5F, 0.5F,  0.5F,
    -1.0F, 0.0F,  0.0F,  1.0F,  0.0F,  -0.5F, 0.5F,  -0.5F, -1.0F, 0.0F,  0.0F,
    1.0F,  1.0F,  -0.5F, -0.5F, -0.5F, -1.0F, 0.0F,  0.0F,  0.0F,  1.0F,  -0.5F,
    -0.5F, -0.5F, -1.0F, 0.0F,  0.0F,  0.0F,  1.0F,  -0.5F, -0.5F, 0.5F,  -1.0F,
    0.0F,  0.0F,  0.0F,  0.0F,  -0.5F, 0.5F,  0.5F,  -1.0F, 0.0F,  0.0F,  1.0F,
    0.0F,  0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,  1.0F,  0.0F,  0.5F,  0.5F,
    -0.5F, 1.0F,  0.0F,  0.0F,  1.0F,  1.0F,  0.5F,  -0.5F, -0.5F, 1.0F,  0.0F,
    0.0F,  0.0F,  1.0F,  0.5F,  -0.5F, -0.5F, 1.0F,  0.0F,  0.0F,  0.0F,  1.0F,
    0.5F,  -0.5F, 0.5F,  1.0F,  0.0F,  0.0F,  0.0F,  0.0F,  0.5F,  0.5F,  0.5F,
    1.0F,  0.0F,  0.0F,  1.0F,  0.0F,  -0.5F, -0.5F, -0.5F, 0.0F,  -1.0F, 0.0F,
    0.0F,  1.0F,  0.5F,  -0.5F, -0.5F, 0.0F,  -1.0F, 0.0F,  1.0F,  1.0F,  0.5F,
    -0.5F, 0.5F,  0.0F,  -1.0F, 0.0F,  1.0F,  0.0F,  0.5F,  -0.5F, 0.5F,  0.0F,
    -1.0F, 0.0F,  1.0F,  0.0F,  -0.5F, -0.5F, 0.5F,  0.0F,  -1.0F, 0.0F,  0.0F,
    0.0F,  -0.5F, -0.5F, -0.5F, 0.0F,  -1.0F, 0.0F,  0.0F,  1.0F,  -0.5F, 0.5F,
    -0.5F, 0.0F,  1.0F,  0.0F,  0.0F,  1.0F,  0.5F,  0.5F,  -0.5F, 0.0F,  1.0F,
    0.0F,  1.0F,  1.0F,  0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,  1.0F,  0.0F,
    0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,  1.0F,  0.0F,  -0.5F, 0.5F,  0.5F,
    0.0F,  1.0F,  0.0F,  0.0F,  0.0F,  -0.5F, 0.5F,  -0.5F, 0.0F,  1.0F,  0.0F,
    0.0F,  1.0F
  };

  GLuint cubeVAO, vbo;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &vbo);

  glBindVertexArray(cubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

  std::size_t verticesElementSize = sizeof(decltype(vertices)::value_type);

  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      8 * verticesElementSize,
      reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1,
      3,
      GL_FLOAT,
      GL_FALSE,
      8 * verticesElementSize,
      reinterpret_cast<void*>(3 * verticesElementSize));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2,
      2,
      GL_FLOAT,
      GL_FALSE,
      8 * verticesElementSize,
      reinterpret_cast<void*>(6 * verticesElementSize));
  glEnableVertexAttribArray(2);

  GLuint lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      8 * verticesElementSize,
      reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glm::vec3 fixedLightPos(1.2F, 1.0F, 2.0F);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    float t = static_cast<float>(glfwGetTime());
    glm::mat4 rotateLight(1.0F);
    rotateLight = glm::rotate(
        rotateLight, glm::radians(16.0F * t), glm::vec3(0.0F, 1.0F, 0.0F));
    glm::vec3 lightPos =
        glm::vec3(rotateLight * glm::vec4(fixedLightPos, 1.0F));

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = cameraProjection.getProjectionMatrix();

    glm::mat4 cubeModel(1.0F);
    cubeModel = glm::translate(cubeModel, glm::vec3(0.0F, 0.0F, 0.0F));
    glm::mat3 normalMat =
        glm::transpose(glm::inverse(glm::mat3(view * cubeModel)));

    glm::mat4 lightCubeModel(1.0f);
    lightCubeModel = glm::translate(lightCubeModel, lightPos);
    lightCubeModel = glm::scale(lightCubeModel, glm::vec3(0.2f));

    // Render Commands Start

    glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldShader.bind();
    worldShader.setVec3("lightPos", lightPos);
    worldShader.setVec3("objectColor", 1.0F, 0.5F, 0.31F);
    worldShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    worldShader.setMat4("view", view);
    worldShader.setMat4("projection", projection);
    worldShader.setMat4("model", cubeModel);
    worldShader.setMat3("normalMat", normalMat);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightShader.bind();
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("model", lightCubeModel);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    // Render Commands End

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &vbo);

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
