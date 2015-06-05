//
// Created by Yury Rakhuba on 01/05/15.
//

#include "Platform.hpp"
#include "path_config.hpp"

TFileReaderPtr Platform::GetReader(std::string const & resourceName)
{
  return TFileReaderPtr(new FileReader(std::string(RESOURCE_PATH) + resourceName));
}
