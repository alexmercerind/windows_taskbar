
/*
 *  This file is part of windows_taskbar
 * (https://github.com/alexmercerind/windows_taskbar).
 *
 *  windows_taskbar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  windows_taskbar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with windows_taskbar. If not, see <https://www.gnu.org/licenses/>.
 *
 *  Copyright 2021, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
 */

#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <vector>

#include <Windows.h>
#include <strsafe.h>
#include <ShObjIdl.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include "include/windows_taskbar/windows_taskbar_plugin.h"

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

  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  flutter::PluginRegistrarWindows* registrar_;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;
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
}

WindowsTaskbarPlugin::~WindowsTaskbarPlugin() {}

void WindowsTaskbarPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  auto arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
  auto buttons = std::get<flutter::EncodableList>(
      arguments[flutter::EncodableValue("buttons")]);
  if (method_call.method_name().compare(kSetThumbnailToolbar) == 0) {
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
              (HICON)LoadImage(
                  0, std::wstring(icon.begin(), icon.end()).c_str(), IMAGE_ICON,
                  GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON),
                  LR_LOADFROMFILE | LR_LOADTRANSPARENT));
        }
        if (image_list) {
          hr = taskbar_list->ThumbBarSetImageList(
              ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT),
              image_list);
          auto thumb_buttons = std::make_unique<THUMBBUTTON[]>(buttons.size());
          if (SUCCEEDED(hr)) {
            for (int32_t i = 0; i < buttons.size(); i++) {
              auto button = buttons[i];
              auto data = std::get<flutter::EncodableMap>(button);
              auto id = std::get<int32_t>(data[flutter::EncodableValue("id")]);
              auto mode =
                  std::get<int32_t>(data[flutter::EncodableValue("mode")]);
              auto tooltip = std::get<std::string>(
                  data[flutter::EncodableValue("tooltip")]);
              thumb_buttons[i].dwMask = THB_BITMAP | THB_TOOLTIP | THB_FLAGS;
              thumb_buttons[i].dwFlags = (THUMBBUTTONFLAGS)mode;
              thumb_buttons[i].iId = (UINT)id;
              thumb_buttons[i].iBitmap = i;
              StringCchCopy(
                  thumb_buttons[i].szTip, ARRAYSIZE(thumb_buttons[i].szTip),
                  std::wstring(tooltip.begin(), tooltip.end()).c_str());
            }
            taskbar_list->ThumbBarAddButtons(
                ::GetAncestor(registrar_->GetView()->GetNativeWindow(),
                              GA_ROOT),
                (UINT)buttons.size(), thumb_buttons.get());
            ImageList_Destroy(image_list);
          }
        }
      }
    }
    result->Success(flutter::EncodableValue(nullptr));
  } else {
    result->NotImplemented();
  }
}
}

void WindowsTaskbarPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  WindowsTaskbarPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
