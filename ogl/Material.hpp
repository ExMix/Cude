//
// Created by Yury Rakhuba on 09/07/15.
//

#pragma once

#include <assimp/material.h>

#include <OpenGL/gl.h>

class Material
{
public:
  Material();

  void Init(aiMaterial * material);
  void Apply(GLuint diffuseSlot, GLuint normalSlot);

private:
  GLuint m_diffuseID;
  GLuint m_normalID;
};


