#pragma once

#ifdef _WIN32
  #ifdef LIBASYNC_BUILD
    #define LIBASYNC_EXPORT __declspec(dllexport)
  #else
    #define LIBASYNC_EXPORT __declspec(dllimport)
  #endif
#else
  #define LIBASYNC_EXPORT
#endif
