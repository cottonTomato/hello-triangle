#ifndef INCLUDE_INCLUDE_TEXTURE_HPP_
#define INCLUDE_INCLUDE_TEXTURE_HPP_

#include <string>

class Texture
{
 private:
  unsigned int textureId;

 public:
  enum Type
  {
    DIFFUSE,
    SPECULAR
  };

  Texture(const std::string& path, Type type);
  ~Texture() noexcept;

  Texture(const Texture& other) = delete;
  Texture& operator=(const Texture& other) = delete;

  Texture(Texture&& other);
  Texture& operator=(Texture&& other);

  unsigned int getId() const noexcept;
  Type getType() const noexcept;
  std::string typeStr() const noexcept;

 private:
  Type textureType;
};

#endif  // INCLUDE_INCLUDE_TEXTURE_HPP_
