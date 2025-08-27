#include "Model.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

Model::Model(const std::string& path)
    : directory(std::filesystem::path(path).parent_path())
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
      path,
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals |
          aiProcess_CalcTangentSpace);

  if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      scene->mRootNode == nullptr)
  {
    using namespace std::string_literals;
    throw std::runtime_error("ERROR::ASSIMP: "s + importer.GetErrorString());
  }

  processNode(scene->mRootNode, scene);
}

void Model::draw(const Shader& shader) const noexcept
{
  for (auto& mesh : meshes)
    mesh.draw(shader);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(mesh, scene);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  TextureVector textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;

    vertex.position = glm::vec3(
        mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    vertex.normal = glm::vec3(
        mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
    if (mesh->mTextureCoords[0] != nullptr)
    {
      vertex.texCoords =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    else
    {
      vertex.texCoords = glm::vec2(0.0F);
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

    loadMaterialTexture(
        mat, aiTextureType_DIFFUSE, Texture::Type::DIFFUSE, textures);
    loadMaterialTexture(
        mat, aiTextureType_SPECULAR, Texture::Type::SPECULAR, textures);
  }

  meshes.emplace_back(
      std::move(vertices), std::move(indices), std::move(textures));
}

void Model::loadMaterialTexture(
    aiMaterial* mat,
    aiTextureType aiTexType,
    Texture::Type texType,
    TextureVector& textures)
{
  for (unsigned int i = 0; i < mat->GetTextureCount(aiTexType); i++)
  {
    aiString str;
    mat->GetTexture(aiTexType, i, &str);

    auto it = loadedTextures.find(str.C_Str());
    if (it != loadedTextures.end())
    {
      textures.push_back(it->second);
    }
    else
    {
      textures.emplace_back(
          std::make_shared<Texture>(directory / str.C_Str(), texType));
      loadedTextures.insert({ str.C_Str(), textures.back() });
    }
  }
}
