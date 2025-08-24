#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <array>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.hpp"
#include "glad/glad.h"
#include "stb/image.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

void errorCallback(int error, const char* description);
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

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

  glEnable(GL_DEPTH_TEST);

  Shader shp("./shaders/vertex.glsl", "./shaders/fragment1.glsl");

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

  shp.use();
  shp.setInt("texture1", 0);

  std::array cubePostitons{
    glm::vec3(0.0F, 0.0F, 0.0F),    glm::vec3(2.0F, 5.0F, -15.0F),
    glm::vec3(-1.5F, -2.2F, -2.5F), glm::vec3(-3.8F, -2.0F, -12.3F),
    glm::vec3(2.4F, -0.4F, -3.5F),  glm::vec3(-1.7F, 3.0F, -7.5F),
    glm::vec3(1.3F, -2.0F, -2.5F),  glm::vec3(1.5F, 2.0F, -2.5F),
    glm::vec3(1.5F, 0.2F, -1.5F),   glm::vec3(-1.3F, 1.0F, -1.5F)
  };

  int modelLoc = glGetUniformLocation(shp.getProgramId(), "model");
  int viewLoc = glGetUniformLocation(shp.getProgramId(), "view");
  int projectionLoc = glGetUniformLocation(shp.getProgramId(), "projection");

  float aspectRatio = static_cast<float>(windowWidth) / windowHeight;
  glm::mat4 projection(1.0F);
  projection = glm::perspective(glm::radians(60.0F), aspectRatio, 0.1F, 100.0F);

  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // Render Commands Start

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId1);

    shp.use();

    float t = static_cast<float>(glfwGetTime());
    float radius = 10.0f;
    float camX = sin(t) * radius;
    float camZ = cos(t) * radius;

    glm::vec3 cameraPos(camX, 0.0F, camZ);
    glm::vec3 cameraLookAt(0.0F, 0.0F, 0.0F);
    glm::vec3 cameraUpDir(0.0F, 1.0F, 0.0F);
    glm::mat4 view = glm::lookAt(cameraPos, cameraLookAt, cameraUpDir);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

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

      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
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
