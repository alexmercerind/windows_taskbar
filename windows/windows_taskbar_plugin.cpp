/// This file is a part of windows_taskbar
/// (https://github.com/alexmercerind/windows_taskbar).
///
/// Copyright (c) 2021 & 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
/// All rights reserved.
/// Use of this source code is governed by MIT license that can be found in the
/// LICENSE file.

#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include "include/windows_taskbar/windows_taskbar_plugin.h"

#include <ShObjIdl.h>
#include <Windows.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <strsafe.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment( \
    linker,      \
    "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace {

// Convert string encoded in UTF-8 to wstring.
// Returns an empty std::wstring on failure.
std::wstring Utf16FromUtf8(std::string string) {
  int size_needed =
      MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, NULL, 0);
  if (size_needed == 0) {
    return std::wstring();
  }
  std::wstring wstrTo(size_needed, 0);
  int converted_length = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1,
                                             &wstrTo[0], size_needed);
  if (converted_length == 0) {
    return std::wstring();
  }
  return wstrTo;
}

class WindowsTaskbarPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  WindowsTaskbarPlugin(flutter::PluginRegistrarWindows* registrar);

  virtual ~WindowsTaskbarPlugin();

 private:
  static constexpr UINT kBaseThumbnailToolbarButtonId = 40001;
  static constexpr UINT kMaximumButtonCount = 7;
  static constexpr auto kSetProgressMode = "SetProgressMode";
  static constexpr auto kSetProgress = "SetProgress";
  static constexpr auto kSetThumbnailToolbar = "SetThumbnailToolbar";
  static constexpr auto kSetThumbnailTooltip = "SetThumbnailTooltip";
  static constexpr auto kSetFlashTaskbar = "SetFlashTaskbar";

  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  flutter::PluginRegistrarWindows* registrar_;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;
  bool thumb_buttons_added_ = false;
};

void WindowsTaskbarPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows* registrar) {
  auto plugin = std::make_unique<WindowsTaskbarPlugin>(registrar);
  registrar->AddPlugin(std::move(plugin));
}

WindowsTaskbarPlugin::WindowsTaskbarPlugin(
    flutter::PluginRegistrarWindows* registrar)
    : registrar_(registrar) {
  channel_ = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      registrar_->messenger(), "com.alexmercerind/windows_taskbar",
      &flutter::StandardMethodCodec::GetInstance());
  channel_->SetMethodCallHandler([this](const auto& call, auto result) {
    HandleMethodCall(call, std::move(result));
  });
  registrar_->RegisterTopLevelWindowProcDelegate(
      [=](HWND hwnd, UINT message, WPARAM wparam,
          LPARAM lparam) -> std::optional<HRESULT> {
        {
          switch (message) {
            case WM_COMMAND: {
              int const button_id = LOWORD(wparam);
              if (button_id > 40000 &&
                  button_id <= 40000 + kMaximumButtonCount) {
                int32_t index = button_id - kBaseThumbnailToolbarButtonId;
                channel_->InvokeMethod(
                    "WM_COMMAND",
                    std::make_unique<flutter::EncodableValue>(index));
              }
              break;
            }
            default:
              break;
          }
          return std::nullopt;
        }
      });
}

WindowsTaskbarPlugin::~WindowsTaskbarPlugin() {}

void WindowsTaskbarPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  auto arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
  if (method_call.method_name().compare(kSetProgressMode) == 0) {
    auto mode = std::get<int32_t>(arguments[flutter::EncodableValue("mode")]);
    ITaskbarList3* taskbar_list;
    HRESULT hr;
    hr = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                            IID_PPV_ARGS(&taskbar_list));
    taskbar_list->SetProgressState(
        ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT),
        static_cast<TBPFLAG>(mode));
    taskbar_list->Release();
    result->Success(flutter::EncodableValue(nullptr));
  } else if (method_call.method_name().compare(kSetProgress) == 0) {
    auto completed = (ULONGLONG)std::get<int32_t>(
        arguments[flutter::EncodableValue("completed")]);
    auto total = (ULONGLONG)std::get<int32_t>(
        arguments[flutter::EncodableValue("total")]);
    ITaskbarList3* taskbar_list;
    HRESULT hr;
    hr = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                            IID_PPV_ARGS(&taskbar_list));
    taskbar_list->SetProgressValue(
        ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT),
        completed, total);
    taskbar_list->Release();
    result->Success(flutter::EncodableValue(nullptr));
  } else if (method_call.method_name().compare(kSetThumbnailToolbar) == 0) {
    auto buttons = std::get<flutter::EncodableList>(
        arguments[flutter::EncodableValue("buttons")]);
    ITaskbarList3* taskbar_list;
    HRESULT hr;
    hr = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                            IID_PPV_ARGS(&taskbar_list));
    if (SUCCEEDED(hr)) {
      hr = taskbar_list->HrInit();
      if (SUCCEEDED(hr)) {
        HIMAGELIST image_list = ::ImageList_Create(
            ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CXSMICON),
            ILC_MASK | ILC_COLOR32, 0, 0);
        for (const auto& button : buttons) {
          auto data = std::get<flutter::EncodableMap>(button);
          auto icon =
              std::get<std::string>(data[flutter::EncodableValue("icon")]);
          ::ImageList_AddIcon(
              image_list,
              (HICON)LoadImage(0, Utf16FromUtf8(icon).c_str(), IMAGE_ICON,
                               GetSystemMetrics(SM_CXSMICON),
                               GetSystemMetrics(SM_CXSMICON),
                               LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        }
        if (image_list) {
          hr = taskbar_list->ThumbBarSetImageList(
              ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT),
              image_list);
          THUMBBUTTON thumb_buttons[kMaximumButtonCount];
          if (SUCCEEDED(hr)) {
            for (UINT i = 0; i < kMaximumButtonCount; i++) {
              if (i < buttons.size()) {
                auto button = buttons[i];
                auto data = std::get<flutter::EncodableMap>(button);
                auto mode =
                    std::get<int32_t>(data[flutter::EncodableValue("mode")]);
                auto tooltip = std::get<std::string>(
                    data[flutter::EncodableValue("tooltip")]);
                thumb_buttons[i].dwMask = THB_BITMAP | THB_TOOLTIP | THB_FLAGS;
                thumb_buttons[i].dwFlags =
                    (THUMBBUTTONFLAGS)mode | THBF_ENABLED;
                thumb_buttons[i].iId = kBaseThumbnailToolbarButtonId + i;
                thumb_buttons[i].iBitmap = i;
                ::StringCchCopy(thumb_buttons[i].szTip,
                                ARRAYSIZE(thumb_buttons[i].szTip),
                                Utf16FromUtf8(tooltip).c_str());
              } else {
                thumb_buttons[i].dwMask = THB_FLAGS;
                thumb_buttons[i].dwFlags = THBF_HIDDEN;
                thumb_buttons[i].iId = kBaseThumbnailToolbarButtonId + i;
              }
            }
            if (!thumb_buttons_added_) {
              taskbar_list->ThumbBarAddButtons(
                  ::GetAncestor(registrar_->GetView()->GetNativeWindow(),
                                GA_ROOT),
                  kMaximumButtonCount, thumb_buttons);
              thumb_buttons_added_ = true;
            } else {
              taskbar_list->ThumbBarUpdateButtons(
                  ::GetAncestor(registrar_->GetView()->GetNativeWindow(),
                                GA_ROOT),
                  kMaximumButtonCount, thumb_buttons);
            }
            ::ImageList_Destroy(image_list);
          }
        }
      }
    }
    taskbar_list->Release();
    result->Success(flutter::EncodableValue(nullptr));
  } else if (method_call.method_name().compare(kSetThumbnailTooltip) == 0) {
    auto tooltip =
        std::get<std::string>(arguments[flutter::EncodableValue("tooltip")]);
    ITaskbarList3* taskbar_list;
    HRESULT hr;
    hr = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                            IID_PPV_ARGS(&taskbar_list));
    taskbar_list->SetThumbnailTooltip(
        ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT),
        Utf16FromUtf8(tooltip).c_str());
    taskbar_list->Release();
    result->Success(flutter::EncodableValue(nullptr));
  } else if (method_call.method_name().compare(kSetFlashTaskbar) == 0) {
    auto mode = std::get<int32_t>(arguments[flutter::EncodableValue("mode")]);
    auto flash_count =
        std::get<int32_t>(arguments[flutter::EncodableValue("flashCount")]);
    auto timeout =
        std::get<int32_t>(arguments[flutter::EncodableValue("timeout")]);
    FLASHWINFO flash_info;
    flash_info.cbSize = sizeof(flash_info);
    flash_info.dwFlags = mode;
    flash_info.dwTimeout = timeout;
    flash_info.hwnd =
        ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT);
    flash_info.uCount = flash_count;
    ::FlashWindowEx(&flash_info);
    result->Success(flutter::EncodableValue(nullptr));
  } else {
    result->NotImplemented();
  }
}
}  // namespace

void WindowsTaskbarPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  WindowsTaskbarPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
