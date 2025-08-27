#ifndef INCLUDE_INCLUDE_MODEL_HPP_
#define INCLUDE_INCLUDE_MODEL_HPP_

#include <assimp/scene.h>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class Model
{
 private:
  using TextureVector = std::vector<std::shared_ptr<Texture>>;
  using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

  std::vector<Mesh> meshes;
  std::filesystem::path directory;
  TextureMap loadedTextures;

 public:
  Model(const std::string& path);

  void draw(const Shader& shader) const noexcept;

 private:
  void processNode(aiNode* node, const aiScene* scene);
  void processMesh(aiMesh* mesh, const aiScene* scene);
  void loadMaterialTexture(
      aiMaterial* mat,
      aiTextureType aiTexType,
      Texture::Type texType,
      TextureVector& textures);
};

#endif  // INCLUDE_INCLUDE_MODEL_HPP_
