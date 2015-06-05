//
// Created by Yury Rakhuba on 01/05/15.
//

#pragma once

#include "macros.hpp"

#include <string>
#include <stdint.h>

class FileReader
{
public:
  DISABLE_COPY_AND_MOVE(FileReader);

  FileReader(std::string const & filePath);

  std::string const & GetFilePath() const;
  size_t GetSize() const;
  void Read(void * buffer, size_t size);

private:
  std::string m_filePath;
  FILE * m_file;
  size_t m_size;
};

using TFileReaderPtr = std::unique_ptr<FileReader>;


