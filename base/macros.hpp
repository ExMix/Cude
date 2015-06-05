//
// Created by Yury Rakhuba on 22/05/15.
//

#pragma once

#define DISABLE_COPY(className) \
  className(const className &) = delete; \
  className & operator=(const className &) = delete

#define DISABLE_MOVE(className) \
  className(className &&) = delete; \
  className & operator=(className &&) = delete

#define DISABLE_COPY_AND_MOVE(className) \
  DISABLE_COPY(className); \
  DISABLE_MOVE(className)
