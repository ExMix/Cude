//
//
// Created by Yury Rakhuba on 09/05/15.

#include "ShaderProgram.hpp"
#include "GLUtils.hpp"
#include "base/assert.hpp"

#include <OpenGL/gl3.h>
#include <vector>
#include <functional>
#include <iostream>

using namespace std::placeholders;

namespace
{

using TGetStatusFn = std::function<void (GLint * result)>;
using TGetLogFn = std::function<void (GLsizei bufSize, GLsizei * length, GLchar * infoLog)>;

void Validate(std::string const & filePath, TGetStatusFn const & statusFn, TGetLogFn const & logFn)
{
  GLint result = GL_FALSE;
  GLCHECK(statusFn(&result));
  if (result == GL_FALSE)
  {
    size_t const BUF_SIZE = 1024;
    GLchar infoLog[BUF_SIZE];
    GLint length = 0;
    GLCHECK(logFn(BUF_SIZE, &length, infoLog));
    ASSERT(false, filePath + " " + std::string(infoLog, length));
  }
}


GLuint LoadShader(TFileReaderPtr & shaderSource, GLenum shaderType)
{
  size_t fileSize = shaderSource->GetSize();

  std::vector<GLchar> buffer;
  buffer.resize(fileSize + 1, 0);
  shaderSource->Read(buffer.data(), fileSize);

  GLuint shaderID = glCreateShader(shaderType);
  GLCHECK_CALL();

  GLchar * source = buffer.data();
  GLCHECK(glShaderSource(shaderID, 1, &source, nullptr));
  GLCHECK(glCompileShader(shaderID));

  using namespace std::placeholders;

  Validate(shaderSource->GetFilePath(),
           std::bind(&glGetShaderiv, shaderID, GL_COMPILE_STATUS, _1),
           std::bind(&glGetShaderInfoLog, shaderID, _1, _2, _3));

  return shaderID;
}

} // namespace

ShaderProgram::ShaderProgram(TFileReaderPtr vertexShader, TFileReaderPtr fragmentShader)
  : m_program(0)
{
  GLuint vsh = LoadShader(vertexShader, GL_VERTEX_SHADER);
  GLuint fsh = LoadShader(fragmentShader, GL_FRAGMENT_SHADER);

  GLuint prg = glCreateProgram();
  GLCHECK_CALL();

  GLCHECK(glAttachShader(prg, vsh));
  GLCHECK(glAttachShader(prg, fsh));
  GLCHECK(glLinkProgram(prg));

  Validate("Program",
           std::bind(&glGetProgramiv, prg, GL_LINK_STATUS, _1),
           std::bind(&glGetProgramInfoLog, prg, _1, _2, _3));

  m_program = prg;
}

void ShaderProgram::Bind()
{
  GLCHECK(glUseProgram(m_program));
}

ShaderProgram::~ShaderProgram()
{
  GLCHECK(glDeleteProgram(m_program));
}

GLint ShaderProgram::GetAttribute(std::string const & name)
{
  GLint location = glGetAttribLocation(m_program, name.c_str());
  GLCHECK_CALL();
  return location;
}

GLint ShaderProgram::GetUniform(std::string const & name)
{
  GLint location = glGetUniformLocation(m_program, name.c_str());
  GLCHECK_CALL();
  return location;
}
