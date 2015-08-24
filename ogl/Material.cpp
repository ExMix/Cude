//
// Created by Yury Rakhuba on 09/07/15.
//

#define STB_IMAGE_IMPLEMENTATION

#include "Material.hpp"
#include "base/assert.hpp"
#include "base/stb_image.h"
#include "base/Platform.hpp"
#include "GLUtils.hpp"

#include <assimp/types.h>
#include <assimp/DefaultLogger.hpp>
#include <OpenGL/gl3.h>

#include <vector>

namespace
{

GLuint LoadTexture(std::string const & name)
{
  TFileReaderPtr reader = Platform::GetReader(name);
  std::vector<uint8_t> fileData;
  fileData.resize(reader->GetSize());
  reader->Read(fileData.data(), fileData.size());

  stbi_set_flip_vertically_on_load(true);
  int x, y, comp;
  uint8_t * imageData = stbi_load_from_memory(fileData.data(), fileData.size(), &x, &y, &comp, 0);

  GLenum format = GL_RGBA;
  switch (comp)
  {
    case 1:
      format = GL_RED;
      break;
    case 2:
      ASSERT(false, "");
      break;
    case 3:
      format = GL_RGB;
      break;
  }

  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, imageData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  stbi_image_free(imageData);
  return texID;
}

} // namespace

Material::Material()
  : m_diffuseID(0)
  , m_normalID(0)
{
}

void Material::Init(aiMaterial * material)
{
  aiString textureName;
  if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName))
    m_diffuseID = LoadTexture(std::string(textureName.C_Str()));

  if (AI_SUCCESS == material->GetTexture(aiTextureType_HEIGHT, 0, &textureName))
    m_normalID = LoadTexture(std::string(textureName.C_Str()));
}

void Material::Apply(GLuint diffuseSlot, GLuint normalSlot)
{
  if (m_diffuseID)
  {
    GLCHECK(glActiveTexture(GL_TEXTURE0 + diffuseSlot));
    GLCHECK(glBindTexture(GL_TEXTURE_2D, m_diffuseID));
  }

  if (m_normalID)
  {
    GLCHECK(glActiveTexture(GL_TEXTURE0 + normalSlot));
    GLCHECK(glBindTexture(GL_TEXTURE_2D, m_normalID));
  }
}
