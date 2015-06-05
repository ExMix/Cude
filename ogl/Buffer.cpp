//
// Created by Yury Rakhuba on 22/05/15.
//

#include "Buffer.hpp"
#include "GLUtils.hpp"

#include "base/assert.hpp"

namespace
{

GLenum Convert(Buffer::Type type)
{
  switch (type)
  {
  case Buffer::Type::VertexBuffer:
    return GL_ARRAY_BUFFER;
  case Buffer::Type::IndexBuffer:
    return GL_ELEMENT_ARRAY_BUFFER;
  }
}

} // namespace

Buffer::Buffer(Buffer::Type type)
  : m_bufferID(0)
  , m_type(type)
{
}

void Buffer::Create()
{
  GLCHECK(glGenBuffers(1, &m_bufferID));
}

bool Buffer::IsCreated()
{
  return m_bufferID != 0;
}

void Buffer::Bind()
{
  ASSERT(IsCreated(), "");
  GLCHECK(glBindBuffer(Convert(m_type), m_bufferID));
}

void Buffer::Destroy()
{
  if (IsCreated())
    GLCHECK(glDeleteBuffers(1, &m_bufferID));
}

void Buffer::Allocate(int byteCount)
{
  Allocate(byteCount, nullptr);
}

void Buffer::Allocate(int byteCount, void * data)
{
  ASSERT(IsCreated(), "");
  GLCHECK(glBufferData(Convert(m_type), byteCount, data, GL_STATIC_DRAW));
}

void Buffer::Write(int offset, int byteCount, void * data)
{
  ASSERT(IsCreated(), "");
  GLCHECK(glBufferSubData(Convert(m_type), offset, byteCount, data));
}
