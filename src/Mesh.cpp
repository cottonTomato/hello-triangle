#include "Mesh.hpp"

#include <memory>
#include <string>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"
#include "glad/glad.h"

Mesh::Mesh(
    std::vector<Vertex>&& vert,
    std::vector<unsigned int>&& ind,
    TextureVector&& tex)
    : vertices(std::move(vert)),
      indices(std::move(ind)),
      textures(std::move(tex))
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(
      GL_ARRAY_BUFFER,
      vertices.size() * sizeof(Vertex),
      vertices.data(),
      GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      indices.size() * sizeof(unsigned int),
      indices.data(),
      GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      reinterpret_cast<void*>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() noexcept
{
  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

Mesh::Mesh(Mesh&& other)
    : vao(other.vao),
      vbo(other.vbo),
      ebo(other.ebo),
      vertices(std::move(other.vertices)),
      indices(std::move(other.indices)),
      textures(std::move(other.textures))
{
  other.vao = 0;
  other.vbo = 0;
  other.ebo = 0;
}

Mesh& Mesh::operator=(Mesh&& other)
{
  if (this != &other)
  {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    textures = std::move(other.textures);

    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
  }
  return *this;
}

void Mesh::draw(const Shader& shader) const
{
  unsigned int diffuseNr = 1, specularNr = 1;

  for (unsigned int i = 0; i < textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    int number;
    switch (textures[i]->getType())
    {
      case Texture::Type::DIFFUSE: number = diffuseNr++; break;
      case Texture::Type::SPECULAR: number = specularNr++; break;
    }

    shader.setInt(
        "material." + textures[i]->typeStr() + std::to_string(number), i);
    glBindTexture(GL_TEXTURE_2D, textures[i]->getId());
  }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(vao);
  glDrawElements(
      GL_TRIANGLES,
      static_cast<unsigned int>(indices.size()),
      GL_UNSIGNED_INT,
      nullptr);
  glBindVertexArray(0);
}
