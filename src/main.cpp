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

GLuint loadTexture(const std::string& path);

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
  std::array cubePositions{
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)
  };

  GLuint cubeVAO, VBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(cubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint diffuseMap = loadTexture("./assets/textures/container2.png");
  GLuint specularMap = loadTexture("./assets/textures/container2_specular.png");

  worldShader.bind();
  worldShader.setInt("material.diffuse", 0);
  worldShader.setInt("material.specular", 1);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = cameraProjection.getProjectionMatrix();

    // Render Commands Start

    glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldShader.bind();
    worldShader.setMat4("view", view);
    worldShader.setMat4("projection", projection);

    worldShader.setFloat("material.shininess", 32.0F);

    worldShader.setVec3("light.position", glm::vec3(0.0F));
    worldShader.setVec3("light.direction", glm::vec3(0.0F, 0.0F, -1.0F));
    worldShader.setFloat("light.innerCutoff", glm::cos(glm::radians(10.0F)));
    worldShader.setFloat("light.outerCutoff", glm::cos(glm::radians(12.4F)));
    worldShader.setVec3("light.ambient", 0.1F, 0.1F, 0.1F);
    worldShader.setVec3("light.diffuse", 0.8F, 0.8F, 0.5F);
    worldShader.setVec3("light.specular", 1.0F, 1.0F, 1.0F);
    worldShader.setFloat("light.kc", 1.0F);
    worldShader.setFloat("light.kl", 0.09F);
    worldShader.setFloat("light.kq", 0.032F);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glBindVertexArray(cubeVAO);

    for (std::size_t i = 0; i < cubePositions.size(); i++)
    {
      float angle = 20.0f * i;
      glm::mat4 cubeModel = glm::mat4(1.0f);
      cubeModel = glm::translate(cubeModel, cubePositions[i]);
      cubeModel = glm::rotate(
          cubeModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      glm::mat3 normalMat =
          glm::transpose(glm::inverse(glm::mat3(view * cubeModel)));

      worldShader.setMat4("model", cubeModel);
      worldShader.setMat3("normalMat", normalMat);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Render Commands End

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteTextures(1, &specularMap);
  glDeleteTextures(1, &diffuseMap);
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &VBO);

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

GLuint loadTexture(const std::string& path)
{
  GLuint textureID;
  glGenTextures(1, &textureID);

  int width, height, nrChannels;

  std::unique_ptr<unsigned char, decltype(&stbi_image_free)> image(
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0),
      stbi_image_free);

  if (image.get() == nullptr)
  {
    throw std::runtime_error("ERROR::STB_IMAGE::LOADING_FAILED: " + path);
  }

  GLenum format = GL_RGB;
  switch (nrChannels)
  {
    case 1: format = GL_RED; break;
    case 2: format = GL_RG; break;
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    default: break;
  }

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      format,
      width,
      height,
      0,
      format,
      GL_UNSIGNED_BYTE,
      image.get());
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  return textureID;
}
