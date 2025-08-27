#include "Texture.hpp"

#include <stdexcept>
#include <string>

#include "glad/glad.h"
#include "stb/image.h"

Texture::Texture(const std::string& path, Type type) : textureType(type)
{
  int width, height, nrChannels;
  std::unique_ptr<unsigned char, decltype(&stbi_image_free)> image(
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0),
      stbi_image_free);

  if (image.get() == nullptr)
  {
    throw std::runtime_error("ERROR::STB_IMAGE::LOADING_FAILED: " + path);
  }

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  GLenum format = GL_RGB;
  switch (nrChannels)
  {
    case 1: format = GL_RED; break;
    case 2: format = GL_RG; break;
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    default: break;
  }

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
}

Texture::~Texture() noexcept
{
  glDeleteTextures(1, &textureId);
}

Texture::Texture(Texture&& other)
    : textureId(other.textureId),
      textureType(other.textureType)
{
  other.textureId = 0;
}

Texture& Texture::operator=(Texture&& other)
{
  if (this != &other)
  {
    glDeleteTextures(1, &textureId);

    textureId = other.textureId;

    other.textureId = 0;
  }
  return *this;
}

GLuint Texture::getId() const noexcept
{
  return textureId;
}

Texture::Type Texture::getType() const noexcept
{
  return textureType;
}

std::string Texture::typeStr() const noexcept
{
  switch (textureType)
  {
    case DIFFUSE: return "texture_diffuse";
    case SPECULAR: return "texture_specular";
  }
}
