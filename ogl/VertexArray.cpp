//
// Created by Yury Rakhuba on 23/08/15.
//

#include "base/assert.hpp"
#include "VertexArray.hpp"
#include "GLUtils.hpp"

void VertexArray::Create()
{
  m_thread = std::this_thread::get_id();
  GLCHECK(glGenVertexArrays(1, &m_id));
}

bool VertexArray::IsCreated()
{
  return m_id != 0;
}

void VertexArray::Destroy()
{
  if (IsCreated())
  {
    ASSERT(m_thread == std::this_thread::get_id(), "");
    GLCHECK(glDeleteVertexArrays(1, &m_id));
  }
}

void VertexArray::Bind()
{
  ASSERT(IsCreated(), "");
  ASSERT(m_thread == std::this_thread::get_id(), "");
  GLCHECK(glBindVertexArray(m_id));
}

void VertexArray::Release()
{
  GLCHECK(glBindVertexArray(0));
}

#define PCAST(x) reinterpret_cast<GLvoid *>(x)

void VertexArray::Build(TVAOLayout const & layout)
{
  ASSERT(IsCreated(), "");
  ASSERT(m_thread == std::this_thread::get_id(), "");
  for (TVAONode const & node : layout)
  {
    Buffer * buffer = node.first;
    buffer->Bind();
    LayoutDesc const & desc = node.second;

    GLCHECK(glEnableVertexAttribArray(desc.m_location));
    GLCHECK(glVertexAttribPointer(desc.m_location, desc.m_componentCount,
                                  desc.m_type, GL_FALSE,
                                  desc.m_stride, PCAST(desc.m_offset)));
  }
}
