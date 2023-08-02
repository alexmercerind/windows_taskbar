/// This file is a part of windows_taskbar
/// (https://github.com/alexmercerind/windows_taskbar).
///
/// Copyright (c) 2021 & onwards, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
/// All rights reserved.
/// Use of this source code is governed by MIT license that can be found in the
/// LICENSE file.

#pragma once

#include "include/windows_taskbar/windows_taskbar_plugin.h"

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include "windows_taskbar.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment( \
    linker,      \
    "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace {

class WindowsTaskbarPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  WindowsTaskbarPlugin(flutter::PluginRegistrarWindows* registrar);

  virtual ~WindowsTaskbarPlugin();

 private:
  static constexpr auto kSetProgressMode = "SetProgressMode";
  static constexpr auto kSetProgress = "SetProgress";
  static constexpr auto kSetThumbnailToolbar = "SetThumbnailToolbar";
  static constexpr auto kResetThumbnailToolbar = "ResetThumbnailToolbar";
  static constexpr auto kSetThumbnailTooltip = "SetThumbnailTooltip";
  static constexpr auto kSetFlashTaskbarAppIcon = "SetFlashTaskbarAppIcon";
  static constexpr auto kResetFlashTaskbarAppIcon = "ResetFlashTaskbarAppIcon";
  static constexpr auto kSetOverlayIcon = "SetOverlayIcon";
  static constexpr auto kResetOverlayIcon = "ResetOverlayIcon";
  static constexpr auto kSetWindowTitle = "SetWindowTitle";
  static constexpr auto kResetWindowTitle = "ResetWindowTitle";
  static constexpr auto kIsTaskbarVisible = "IsTaskbarVisible";

  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  std::string GetErrorString(std::string method_name);

  flutter::PluginRegistrarWindows* registrar_;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;
  std::unique_ptr<WindowsTaskbar> windows_taskbar_ = nullptr;
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
              if (button_id >= kMinThumbButtonId &&
                  button_id < kMinThumbButtonId + kMaxThumbButtonCount) {
                int32_t index = button_id - kMinThumbButtonId;
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
  if (windows_taskbar_ == nullptr) {
    windows_taskbar_ = std::make_unique<WindowsTaskbar>(
        ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT));
  }
  auto arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
  if (method_call.method_name().compare(kSetProgressMode) == 0) {
    auto mode = std::get<int32_t>(arguments[flutter::EncodableValue("mode")]);
    if (windows_taskbar_->SetProgressMode(mode)) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kSetProgressMode));
    }
  } else if (method_call.method_name().compare(kSetProgress) == 0) {
    auto completed =
        std::get<int32_t>(arguments[flutter::EncodableValue("completed")]);
    auto total = std::get<int32_t>(arguments[flutter::EncodableValue("total")]);
    if (windows_taskbar_->SetProgress(completed, total)) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kSetProgress));
    }
  } else if (method_call.method_name().compare(kSetThumbnailToolbar) == 0) {
    auto buttons = std::get<flutter::EncodableList>(
        arguments[flutter::EncodableValue("buttons")]);
    std::vector<ThumbnailToolbarButton> thumbnail_toolbar_buttons;
    thumbnail_toolbar_buttons.reserve(buttons.size());
    for (auto const& button : buttons) {
      auto data = std::get<flutter::EncodableMap>(button);
      auto icon = std::get<std::string>(data[flutter::EncodableValue("icon")]);
      auto tooltip =
          std::get<std::string>(data[flutter::EncodableValue("tooltip")]);
      auto mode = std::get<int32_t>(data[flutter::EncodableValue("mode")]);
      ThumbnailToolbarButton thumbnail_toolbar_button;
      thumbnail_toolbar_button.icon = icon;
      thumbnail_toolbar_button.tooltip = tooltip;
      thumbnail_toolbar_button.mode = mode;
      thumbnail_toolbar_buttons.emplace_back(thumbnail_toolbar_button);
    }
    if (windows_taskbar_->SetThumbnailToolbar(thumbnail_toolbar_buttons)) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kSetThumbnailToolbar));
    }
  } else if (method_call.method_name().compare(kResetThumbnailToolbar) == 0) {
    if (windows_taskbar_->ResetThumbnailToolbar()) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kResetThumbnailToolbar));
    }
  } else if (method_call.method_name().compare(kSetThumbnailTooltip) == 0) {
    auto tooltip =
        std::get<std::string>(arguments[flutter::EncodableValue("tooltip")]);
    if (windows_taskbar_->SetThumbnailTooltip(tooltip)) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kSetThumbnailTooltip));
    }
  } else if (method_call.method_name().compare(kSetFlashTaskbarAppIcon) == 0) {
    auto mode = std::get<int32_t>(arguments[flutter::EncodableValue("mode")]);
    auto flash_count =
        std::get<int32_t>(arguments[flutter::EncodableValue("flashCount")]);
    auto timeout =
        std::get<int32_t>(arguments[flutter::EncodableValue("timeout")]);
    if (windows_taskbar_->SetFlashTaskbarAppIcon(mode, flash_count, timeout)) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kSetFlashTaskbarAppIcon));
    }
  } else if (method_call.method_name().compare(kResetFlashTaskbarAppIcon) ==
             0) {
    if (windows_taskbar_->ResetFlashTaskbarAppIcon()) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kResetFlashTaskbarAppIcon));
    }
  } else if (method_call.method_name().compare(kSetOverlayIcon) == 0) {
    auto icon =
        std::get<std::string>(arguments[flutter::EncodableValue("icon")]);
    auto tooltip =
        std::get<std::string>(arguments[flutter::EncodableValue("tooltip")]);
    if (windows_taskbar_->SetOverlayIcon(icon, tooltip)) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kSetOverlayIcon));
    }
  } else if (method_call.method_name().compare(kResetOverlayIcon) == 0) {
    if (windows_taskbar_->ResetOverlayIcon()) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kResetOverlayIcon));
    }
  } else if (method_call.method_name().compare(kSetWindowTitle) == 0) {
    auto title =
        std::get<std::string>(arguments[flutter::EncodableValue("title")]);
    if (windows_taskbar_->SetWindowTitle(title)) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kSetWindowTitle));
    }
  } else if (method_call.method_name().compare(kResetWindowTitle) == 0) {
    if (windows_taskbar_->ResetWindowTitle()) {
      result->Success(flutter::EncodableValue(nullptr));
    } else {
      result->Error("-1", GetErrorString(kResetWindowTitle));
    }
  } else if (method_call.method_name().compare(kIsTaskbarVisible) == 0) {
    bool value = windows_taskbar_->IsTaskbarVisible();
    result->Success(flutter::EncodableValue(value));
  } else {
    result->NotImplemented();
  }
}
}  // namespace

std::string WindowsTaskbarPlugin::GetErrorString(std::string method_name) {
  return "WindowsTaskbar::" + method_name + " call failed.";
}

void WindowsTaskbarPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  WindowsTaskbarPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
