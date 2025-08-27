#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <stdexcept>

#include "Camera.hpp"
#include "Model.hpp"
#include "Projection.hpp"
#include "Shader.hpp"
#include "glad/glad.h"
#include "stb/image.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;
constexpr float aspectRatio = static_cast<float>(windowWidth) / windowHeight;

Camera camera = CameraBuilder().setPosition(0.0F, 0.0F, 3.0F).build();
Projection projection =
    ProjectionBuilder().withAspectRatio(aspectRatio).build();

void errorCallback(int error, const char* description);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

struct GlfwContext
{
  GlfwContext()
  {
    if (!glfwInit())
      throw std::runtime_error("Failed to init GLFW.");
  }

  ~GlfwContext()
  {
    glfwTerminate();
  }
};

int main()
{
  glfwSetErrorCallback(errorCallback);

  GlfwContext glfwContext;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(
      windowWidth, windowHeight, "Hello OpenGL", nullptr, nullptr);
  if (window == nullptr)
    throw std::runtime_error("Failed to create GLFW window.");

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    throw std::runtime_error("Failed to initialize GLAD");

  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);

  Shader worldShader("./shaders/vertex2.glsl", "./shaders/fragment2.glsl");

  Model backpackModel("./assets/models/backpack/backpack.obj");

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    glm::mat4 projectionMatrix = projection.getProjectionMatrix();
    glm::mat4 viewMatrix = camera.getViewMatrix();

    // Render Commands Start

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldShader.bind();
    worldShader.setMat4("projection", projectionMatrix);
    worldShader.setMat4("view", viewMatrix);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    worldShader.setMat4("model", model);

    backpackModel.draw(worldShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
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
    camera.updatePosition(Camera::MoveDir::FORWARD, deltaFrame);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.updatePosition(Camera::MoveDir::BACKWARD, deltaFrame);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.updatePosition(Camera::MoveDir::LEFT, deltaFrame);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.updatePosition(Camera::MoveDir::RIGHT, deltaFrame);
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
  projection.updateFov(static_cast<float>(yoffset));
}

void frameBufferSizeCallback(
    [[maybe_unused]] GLFWwindow* window,
    int width,
    int height)
{
  glViewport(0, 0, width, height);
}

void errorCallback(int error, const char* description)
{
  std::ostringstream oss;
  oss << "ERROR::GLFW (" << error << "): " << description;

  throw std::runtime_error(oss.str());
}
