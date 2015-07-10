//
// Created by Yury Rakhuba on 22/05/15.
//

#pragma once

#include <OpenGL/gl.h>

class Buffer
{
public:
  enum class Type
  {
    VertexBuffer,
    IndexBuffer
  };

  explicit Buffer(Type type);
  ~Buffer();

  void Create();
  bool IsCreated();
  void Bind();
  void Destroy();

  void Allocate(int byteCount);
  void Allocate(int byteCount, void * data);

  void Write(int offset, int byteCount, void * data);

private:
  GLuint m_bufferID;
  Type m_type;
};


