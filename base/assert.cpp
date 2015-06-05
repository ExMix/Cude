//
// Created by Yury Rakhuba on 09/05/15.
//

#include "assert.hpp"

#include <iostream>
#include <sstream>
#include <assert.h>

namespace impl
{

SourcePoint::SourcePoint(char const * fileName, int lineNumber, char const * functionName)
  : m_lineNumber(lineNumber)
  , m_functionName(functionName)
{
  SetFileName(fileName);
}
std::string const & SourcePoint::GetFileName() const
{
  return m_fileName;
}

int SourcePoint::GetLineNumber() const
{
  return m_lineNumber;
}

std::string const & SourcePoint::GetFunctionName() const
{
  return m_functionName;
}

void SourcePoint::SetFileName(char const * fileName)
{
  char const * result = nullptr;
  int counter = 0;
  size_t length = strlen(fileName);
  for (size_t i = length - 1; i > 0 && counter < 2; --i)
  {
    if (fileName[i] == '\\' || fileName[i]== '/')
    {
      result = fileName + i + 1;
      ++counter;
    }
  }

  if (result != nullptr)
    m_fileName = std::string(result);
}

std::string DebugFormat(SourcePoint const & pt)
{
  std::ostringstream ss;
  ss << pt.GetFileName() << " (" << pt.GetLineNumber() << ")" << " : " << pt.GetFunctionName();
  return ss.str();
}

} // namespace impl


void AssertFunction(impl::SourcePoint const & pt, std::string const & msg)
{
  std::cerr << impl::DebugFormat(pt) << " " << msg << std::endl;
  assert(false);
}
