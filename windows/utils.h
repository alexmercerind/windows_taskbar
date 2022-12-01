/// This file is a part of windows_taskbar
/// (https://github.com/alexmercerind/windows_taskbar).
///
/// Copyright (c) 2021 & onwards, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
/// All rights reserved.
/// Use of this source code is governed by MIT license that can be found in the
/// LICENSE file.

#ifndef UTILS_H_
#define UTILS_H_

#include <strsafe.h>

static inline std::wstring Utf16FromUtf8(std::string string) {
  int size_needed =
      ::MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, NULL, 0);
  if (size_needed == 0) {
    return std::wstring();
  }
  std::wstring result(size_needed, 0);
  int converted_length = ::MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1,
                                               &result[0], size_needed);
  if (converted_length == 0) {
    return std::wstring();
  }
  return result;
}

#endif
