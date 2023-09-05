/// This file is a part of windows_taskbar
/// (https://github.com/alexmercerind/windows_taskbar).
///
/// Copyright (c) 2021 & onwards, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
/// All rights reserved.
/// Use of this source code is governed by MIT license that can be found in the
/// LICENSE file.

#include "utils.h"

#include <Windows.h>

std::wstring Utf16FromUtf8(std::string string) {
  auto size_needed =
      ::MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, NULL, 0);
  if (size_needed == 0) {
    return std::wstring{};
  }
  auto result = std::wstring(size_needed, 0);
  auto converted_length = ::MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1,
                                                &result[0], size_needed);
  if (converted_length == 0) {
    return std::wstring{};
  }
  return result;
}
