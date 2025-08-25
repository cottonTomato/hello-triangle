#ifndef INCLUDE_INCLUDE_TEXTURE_HPP_
#define INCLUDE_INCLUDE_TEXTURE_HPP_

#include <string>

#include "Shader.hpp"

class Texture
{
 private:
  unsigned int textureId;

 public:
  Texture(const std::string& path);

  unsigned int getId() const;

  void bind(unsigned int unit) const;

  void unbind(unsigned int unit);
};

#endif  // INCLUDE_INCLUDE_TEXTURE_HPP_
