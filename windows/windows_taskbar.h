/// This file is a part of windows_taskbar
/// (https://github.com/alexmercerind/windows_taskbar).
///
/// Copyright (c) 2021 & onwards, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
/// All rights reserved.
/// Use of this source code is governed by MIT license that can be found in the
/// LICENSE file.

#ifndef WINDOWS_TASKBAR_H_
#define WINDOWS_TASKBAR_H_

#include <ShObjIdl.h>
#include <Windows.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct ThumbnailToolbarButton {
  std::string icon;
  std::string tooltip;
  int32_t mode;
};

static constexpr auto kMinThumbButtonId = 40001;
static constexpr auto kMaxThumbButtonCount = 7;

class WindowsTaskbar {
 public:
  WindowsTaskbar(HWND window, bool ensure_visibility = true);

  ~WindowsTaskbar();

  bool SetProgressMode(int32_t mode);

  bool SetProgress(int32_t completed, int32_t total);

  bool SetThumbnailToolbar(std::vector<ThumbnailToolbarButton> buttons);

  bool ResetThumbnailToolbar();

  bool SetThumbnailTooltip(std::string tooltip);

  bool SetFlashTaskbarAppIcon(int32_t mode, int32_t flash_count,
                              int32_t timeout);

  bool ResetFlashTaskbarAppIcon();

  bool SetOverlayIcon(std::string icon, std::string tooltip);

  bool ResetOverlayIcon();

  bool SetWindowTitle(std::string title);

  bool ResetWindowTitle();

 private:
  HWND window_ = nullptr;
  bool ensure_visibility_ = false;
  ITaskbarList3* taskbar_ = nullptr;
  bool thumb_buttons_added_ = false;
  std::unique_ptr<wchar_t[]> window_title_ = nullptr;
};

#endif
