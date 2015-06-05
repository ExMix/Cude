//
// Created by Yury Rakhuba on 09/05/15.
//

#pragma once

#include "base/FileReader.hpp"
#include "base/macros.hpp"

#include <OpenGL/gl.h>

class ShaderProgram
{
public:
  DISABLE_MOVE(ShaderProgram);

  ShaderProgram(TFileReaderPtr vertexShader, TFileReaderPtr fragmentShader);
  ~ShaderProgram();

  GLint GetAttribute(std::string const & name);
  GLint GetUniform(std::string const & name);

  void Bind();

private:
  int m_program;
};


