//
// Created by Yury Rakhuba on 23/08/15.
//

#pragma once

#include <stdio.h>

enum ESemantic
{
  kPositionLoc = 0,
  kNormalLoc = 1,
  kTangentLoc = 2,
  kBitangentLoc = 3,
  kTexCoordLoc = 4
};

struct LayoutDesc
{
  ESemantic m_location;
  size_t m_componentCount;
  size_t m_type;
  size_t m_stride;
  size_t m_offset;
};
