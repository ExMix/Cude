//
// Created by Yury Rakhuba on 23/08/15.
//

#pragma once

#include "Layout.hpp"
#include "Buffer.hpp"

#include <utility>
#include <vector>
#include <thread>
#include <OpenGL/gl3.h>

class VertexArray
{
public:
  void Create();
  bool IsCreated();
  void Destroy();
  void Bind();

  static void Release();

  using TVAONode = std::pair<Buffer *, LayoutDesc>;
  using TVAOLayout = std::vector<TVAONode>;

  void Build(TVAOLayout const & layout);

private:
  std::thread::id m_thread;
  GLuint m_id = 0;
};


