//
// Created by Yury Rakhuba on 01/05/15.
//

#pragma once

#include "FileReader.hpp"

#include <memory>

class Platform
{
public:
  static TFileReaderPtr GetReader(std::string const & resourceName);
};


