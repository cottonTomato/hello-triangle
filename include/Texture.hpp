#ifndef INCLUDE_INCLUDE_TEXTURE_HPP_
#define INCLUDE_INCLUDE_TEXTURE_HPP_

#include <string>

class Texture
{
 private:
  unsigned int textureId;

 public:
  Texture(const std::string& path);

  unsigned int getId() const;
};

#endif  // INCLUDE_INCLUDE_TEXTURE_HPP_
