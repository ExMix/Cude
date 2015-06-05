//
// Created by Yury RAkhuba on 01/05/15.
//

#include "FileReader.hpp"

#include <ios>

FileReader::FileReader(std::string const & filePath)
  : m_filePath(filePath)
{
  m_file = fopen(m_filePath.c_str(), "r");
  if (m_file == 0)
    throw std::ios_base::failure((filePath + " " + strerror(errno)).c_str());

  fseek(m_file, 0, SEEK_END);
  m_size = ftell(m_file);
  fseek(m_file, 0, SEEK_SET);
}

size_t FileReader::GetSize() const
{
  return m_size;
}

void FileReader::Read(void * buffer, size_t size)
{
  size_t readed = fread(buffer, 1, size, m_file);
  if (size == readed)
    return;

  throw std::ios_base::failure(strerror(errno));
}

std::string const & FileReader::GetFilePath() const
{
  return m_filePath;
}
