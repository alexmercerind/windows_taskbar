/// This file is a part of windows_taskbar
/// (https://github.com/alexmercerind/windows_taskbar).
///
/// Copyright (c) 2021 & onwards, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
/// All rights reserved.
/// Use of this source code is governed by MIT license that can be found in the
/// LICENSE file.

#include "windows_taskbar.h"

#include <WinUser.h>
#include <strsafe.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <comdef.h>

#include "utils.h"

#define LOG_ERROR(msg) { \
  std::stringstream ss; \
  ss << msg; \
  last_error_ = ss.str(); \
  std::cerr << "WindowsTaskbar Error: " << last_error_ << std::endl; \
}
#define LOG_HRESULT(msg, hr) { \
  std::stringstream ss; \
  ss << msg << " (HRESULT: 0x" << std::hex << (uint32_t)hr << std::dec << " - " << Utf8FromUtf16(_com_error(hr).ErrorMessage()) << ")"; \
  last_error_ = ss.str(); \
  std::cerr << "WindowsTaskbar Error: " << last_error_ << std::endl; \
}

WindowsTaskbar::WindowsTaskbar(HWND window) : window_(window) {
  auto result = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                                   IID_PPV_ARGS(&taskbar_));
  if (FAILED(result)) {
    LOG_HRESULT("CoCreateInstance(CLSID_TaskbarList) failed", result);
  } else {
    result = taskbar_->HrInit();
    if (FAILED(result)) {
      LOG_HRESULT("ITaskbarList3::HrInit failed", result);
    }
  }
}

WindowsTaskbar::~WindowsTaskbar() {
  if (taskbar_) {
    taskbar_->Release();
    taskbar_ = nullptr;
  }
}

bool WindowsTaskbar::SetProgressMode(int32_t mode) {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("SetProgressMode failed: Window is not visible.");
    return false;
  }

  if (taskbar_) {
    auto result =
        taskbar_->SetProgressState(window_, static_cast<TBPFLAG>(mode));
    if (FAILED(result)) {
      LOG_HRESULT("SetProgressState failed", result);
    }
    return SUCCEEDED(result);
  }
  LOG_ERROR("SetProgressMode failed: taskbar_ is NULL.");
  return false;
}

bool WindowsTaskbar::SetProgress(int32_t completed, int32_t total) {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("SetProgress failed: Window is not visible.");
    return false;
  }

  if (taskbar_) {
    auto result = taskbar_->SetProgressValue(window_, completed, total);
    if (FAILED(result)) {
      LOG_HRESULT("SetProgressValue failed", result);
    }
    return SUCCEEDED(result);
  }
  LOG_ERROR("SetProgress failed: taskbar_ is NULL.");
  return false;
}

bool WindowsTaskbar::SetThumbnailToolbar(
    std::vector<ThumbnailToolbarButton> buttons) {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("SetThumbnailToolbar failed: Window is not visible.");
    return false;
  }

  if (buttons.size() > kMaxThumbButtonCount) {
    LOG_ERROR("SetThumbnailToolbar failed: Too many buttons (" << buttons.size() << "). Max is " << kMaxThumbButtonCount << ".");
    return false;
  }

  if (taskbar_) {
    auto image_list = ::ImageList_Create(::GetSystemMetrics(SM_CXSMICON),
                                         ::GetSystemMetrics(SM_CXSMICON),
                                         ILC_MASK | ILC_COLOR32, 0, 0);
    if (!image_list) {
      LOG_ERROR("SetThumbnailToolbar failed: ImageList_Create returned NULL.");
    }
    // Add all images to the |image_list| & set using |ThumbBarSetImageList|.
    for (const auto& button : buttons) {
      // Using |IMAGE_ICON| as default image type since it allows
      // transparency.
      auto icon = (HICON)::LoadImage(0, Utf16FromUtf8(button.icon).c_str(), IMAGE_ICON,
                                     GetSystemMetrics(SM_CXSMICON),
                                     GetSystemMetrics(SM_CXSMICON),
                                     LR_LOADFROMFILE | LR_LOADTRANSPARENT);
      if (icon) {
        ::ImageList_AddIcon(image_list, icon);
        ::DestroyIcon(icon);
      } else {
        LOG_ERROR("SetThumbnailToolbar warning: Failed to load icon: " << button.icon);
      }
    }
    if (image_list) {
      auto result = taskbar_->ThumbBarSetImageList(window_, image_list);
      if (FAILED(result)) {
        LOG_HRESULT("ThumbBarSetImageList failed", result);
      }
      // |ITaskbarList3| can have a maximum of 7 buttons.
      // The number of buttons set using |ThumbBarAddButtons| cannot be
      // changed afterwards during whole window's lifecycle. Thus, setting
      // maximum i.e. 7 buttons on every call & adding |THBF_HIDDEN| flag to
      // hide the remaining additional buttons. On subsequent calls,
      // |ThumbBarUpdateButtons| is used to update the thumbnail toolbar
      // buttons again adding |THBF_HIDDEN| flag to remaining additional
      // buttons.
      THUMBBUTTON thumb_buttons[kMaxThumbButtonCount];
      if (SUCCEEDED(result)) {
        for (uint32_t i = 0; i < kMaxThumbButtonCount; i++) {
          // Adding required buttons with |THBF_ENABLED| flag at the start of
          // |thumb_buttons|.
          if (i < buttons.size()) {
            auto button = buttons[i];
            thumb_buttons[i].dwMask = THB_BITMAP | THB_TOOLTIP | THB_FLAGS;
            thumb_buttons[i].dwFlags =
                (THUMBBUTTONFLAGS)buttons[i].mode | THBF_ENABLED;
            thumb_buttons[i].iId = kMinThumbButtonID + i;
            thumb_buttons[i].iBitmap = i;
            ::StringCchCopy(thumb_buttons[i].szTip,
                            ARRAYSIZE(thumb_buttons[i].szTip),
                            Utf16FromUtf8(buttons[i].tooltip).c_str());
          }
          // Adding remaining buttons with |THBF_HIDDEN| flag.
          else {
            thumb_buttons[i].dwMask = THB_FLAGS;
            thumb_buttons[i].dwFlags = THBF_HIDDEN;
            thumb_buttons[i].iId = kMinThumbButtonID + i;
          }
        }
        // First call, thus using |ThumbBarAddButtons|.
        if (!thumb_buttons_added_) {
          result = taskbar_->ThumbBarAddButtons(window_, kMaxThumbButtonCount,
                                                thumb_buttons);
          if (FAILED(result)) {
            LOG_HRESULT("ThumbBarAddButtons failed", result);
          } else {
            thumb_buttons_added_ = true;
          }
        } else {
          result = taskbar_->ThumbBarUpdateButtons(
              window_, kMaxThumbButtonCount, thumb_buttons);
          if (FAILED(result)) {
            LOG_HRESULT("ThumbBarUpdateButtons failed", result);
          }
        }
        if (SUCCEEDED(result)) {
          ::ImageList_Destroy(image_list);
          return true;
        }
      }
      // If we reached here, something failed. Cleanup image_list.
      ::ImageList_Destroy(image_list);
    } else {
       LOG_ERROR("SetThumbnailToolbar failed: image_list is NULL.");
    }
  } else {
    LOG_ERROR("SetThumbnailToolbar failed: taskbar_ (ITaskbarList3) is NULL.");
  }
  return false;
}

bool WindowsTaskbar::ResetThumbnailToolbar() {
  last_error_ = "";
  return WindowsTaskbar::SetThumbnailToolbar({});
}

bool WindowsTaskbar::SetThumbnailTooltip(std::string tooltip) {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("SetThumbnailTooltip failed: Window is not visible.");
    return false;
  }

  if (taskbar_) {
    auto result =
        taskbar_->SetThumbnailTooltip(window_, Utf16FromUtf8(tooltip).c_str());
    return SUCCEEDED(result);
  }
  return false;
}

bool WindowsTaskbar::SetFlashTaskbarAppIcon(int32_t mode, int32_t flash_count,
                                            int32_t timeout) {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("SetFlashTaskbarAppIcon failed: Window is not visible.");
    return false;
  }

  FLASHWINFO flash_info;
  flash_info.cbSize = sizeof(flash_info);
  flash_info.dwFlags = mode;
  flash_info.dwTimeout = timeout;
  flash_info.hwnd = window_;
  flash_info.uCount = flash_count;
  auto result = ::FlashWindowEx(&flash_info);
  return SUCCEEDED(result);
}

bool WindowsTaskbar::ResetFlashTaskbarAppIcon() {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("ResetFlashTaskbarAppIcon failed: Window is not visible.");
    return false;
  }

  FLASHWINFO flash_info;
  flash_info.cbSize = sizeof(flash_info);
  flash_info.dwFlags = FLASHW_STOP;
  flash_info.dwTimeout = 0;
  flash_info.hwnd = window_;
  flash_info.uCount = 0;
  auto result = ::FlashWindowEx(&flash_info);
  return SUCCEEDED(result);
}

bool WindowsTaskbar::SetOverlayIcon(std::string icon, std::string tooltip) {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("SetOverlayIcon failed: Window is not visible.");
    return false;
  }

  if (taskbar_) {
    // Using |IMAGE_ICON|.
    auto image = (HICON)LoadImage(0, Utf16FromUtf8(icon).c_str(), IMAGE_ICON,
                                  GetSystemMetrics(SM_CXSMICON),
                                  GetSystemMetrics(SM_CXSMICON),
                                  LR_LOADFROMFILE | LR_LOADTRANSPARENT);
    auto result = taskbar_->SetOverlayIcon(window_, image,
                                           Utf16FromUtf8(tooltip).c_str());
    if (image) {
      ::DestroyIcon(image);
    }
    return SUCCEEDED(result);
  }
  return false;
}

bool WindowsTaskbar::ResetOverlayIcon() {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("ResetOverlayIcon failed: Window is not visible.");
    return false;
  }

  if (taskbar_) {
    auto result = taskbar_->SetOverlayIcon(window_, NULL, L"");
    return SUCCEEDED(result);
  }
  return false;
}

bool WindowsTaskbar::SetWindowTitle(std::string title) {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("SetWindowTitle failed: Window is not visible.");
    return false;
  }

  if (window_title_ == nullptr) {
    window_title_ =
        std::make_unique<wchar_t[]>(::GetWindowTextLengthW(window_) + 1);
    ::GetWindowTextW(window_, window_title_.get(),
                     ::GetWindowTextLengthW(window_) + 1);
  }
  return ::SetWindowTextW(window_, Utf16FromUtf8(title).c_str());
  return false;
}

bool WindowsTaskbar::ResetWindowTitle() {
  last_error_ = "";
  if (!::IsWindowVisible(window_)) {
    LOG_ERROR("ResetWindowTitle failed: Window is not visible.");
    return false;
  }

  if (window_title_ != nullptr) {
    return ::SetWindowTextW(window_, window_title_.get());
  }
  return true;
}

bool WindowsTaskbar::IsTaskbarVisible() {
  auto taskbar = ::FindWindow(L"Shell_TrayWnd", NULL);
  auto monitor = ::MonitorFromWindow(taskbar, MONITOR_DEFAULTTONEAREST);
  auto monitor_info = MONITORINFO{};
  monitor_info.cbSize = sizeof(monitor_info);

  if (::GetMonitorInfo(monitor, &monitor_info)) {
    auto rect = RECT{};
    ::GetWindowRect(taskbar, &rect);
    if ((rect.top >= monitor_info.rcMonitor.bottom - 4) || (rect.right <= 2) ||
        (rect.bottom <= 4) || (rect.left >= monitor_info.rcMonitor.right - 2)) {
      return false;
    }
  }

  return true;
}
