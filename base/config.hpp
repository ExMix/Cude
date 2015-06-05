//
// Created by Yury Rakhuba on 10/05/15.
//

#pragma once


#if defined(DEBUG) || defined(_DEBUG) || defined(NRELEASE) || defined(_NRELEASE)
  #define MDEBUG 1
#else
  #define MDEBUG 0
#endif

#if defined(RELEASE) || defined(_RELEASE) || defined(NDEBUG) || defined(_NDEBUG)
  #define MRELEASE 1
#else
  #define MRELEASE 0
#endif

static_assert(!(MDEBUG && MRELEASE), "You can't define Release and Debug at the same time");

#if !MRELEASE
  #ifndef DEBUG
    #define DEBUG 1
  #endif
#endif
