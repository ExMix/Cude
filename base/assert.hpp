//
// Created by Yury Rakhuba on 09/05/15.
//

#pragma once

#include "config.hpp"

#include <string>

namespace impl
{

class SourcePoint
{
public:
  SourcePoint(char const * fileName, int lineNumber, char const * functionName);

  std::string const & GetFileName() const;
  int GetLineNumber() const;
  std::string const & GetFunctionName() const;

private:
  void SetFileName(char const * fileName);

private:
  int m_lineNumber;
  std::string m_functionName;
  std::string m_fileName;
};

std::string DebugFormat(impl::SourcePoint const & pt);

} // namespace impl

void AssertFunction(impl::SourcePoint const & pt, std::string const & msg);

#define SRC_POINT() impl::SourcePoint(__FILE__, __LINE__, __FUNCTION__)
#define CHECK(X, msg) do{ if (!(X)) AssertFunction(SRC_POINT(), (msg)); } while(false)

#ifdef DEBUG
  #define ASSERT(X, msg) CHECK(X, msg)
#else
  #define ASSERT(X, msg)
#endif

#define FAIL(msg) ASSERT(false, msg)



