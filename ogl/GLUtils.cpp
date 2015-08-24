//
// Created by Yury Rakhuba on 09/05/15.
//

#include "GLUtils.hpp"
#include "base/assert.hpp"

#include <OpenGL/gl3.h>

std::string GetGLError(GLenum error)
{
  switch (error)
  {
  case GL_INVALID_ENUM :                  return "GL_INVALID_ENUM";
  case GL_INVALID_OPERATION :             return "GL_INVALID_OPERATION";
  case GL_INVALID_VALUE :                 return "GL_INVALID_VALUE ";
  case GL_INVALID_FRAMEBUFFER_OPERATION : return "GL_INVALID_FRAMEBUFFER_OPERATION";
  default: ASSERT(false, "Invalid glEnum");
  };

  return "";
}

void GLCheck()
{
  GLenum error = glGetError();
  while (error != GL_NO_ERROR)
  {
    // TODO change assert on logging
    ASSERT(false, GetGLError(error));
    error = glGetError();
  }
}
