//
// Created by Yury Rakhuba on 09/05/15.
//

#pragma once

#include "base/config.hpp"

void GLCheck();

#ifdef DEBUG
  #define GLCHECK(X) do { (X); GLCheck(); } while(false)
  #define GLCHECK_CALL() GLCheck()
#else
  #define GLCHECK(X)
  #define GLCHECK_CALL()
#endif


