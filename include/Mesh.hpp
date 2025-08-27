#ifndef INCLUDE_INCLUDE_MESH_HPP_
#define INCLUDE_INCLUDE_MESH_HPP_

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

class Mesh
{
 private:
  using TextureVector = std::vector<std::shared_ptr<Texture>>;

  unsigned int vao, vbo, ebo;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  TextureVector textures;

 public:
  Mesh(
      std::vector<Vertex>&& vertices,
      std::vector<unsigned int>&& indices,
      TextureVector&& textures);
  ~Mesh() noexcept;

  Mesh(const Mesh& other) = delete;
  Mesh& operator=(const Mesh& other) = delete;

  Mesh(Mesh&& other);
  Mesh& operator=(Mesh&& other);

  void draw(const Shader& shader) const;
};

#endif  // INCLUDE_INCLUDE_MESH_HPP_
