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

import 'dart:io';

import 'package:flutter/services.dart';
import 'package:path/path.dart';

const String _kSetProgressMode = "SetProgressMode";
const String _kSetProgress = "SetProgress";

/// Sets thumbnail toolbar.
const String _kSetThumbnailToolbar = "SetThumbnailToolbar";

/// Method channel for making native WIN32 calls.
final MethodChannel _kChannel =
    const MethodChannel('com.alexmercerind/windows_taskbar')
      ..setMethodCallHandler((call) async {
        switch (call.method) {
          case 'WM_COMMAND':
            {
              _buttons[call.arguments].onClick.call();
              break;
            }
          default:
            break;
        }
      });

/// Declares mode of a [ThumbnailToolbarButton].
/// Analog of `THUMBBUTTONFLAGS` in WIN32 API.
///
class ThumbnailToolbarButtonMode {
  static const int disabled = 0x1;
  static const int dismissionClick = 0x2;
  static const int noBackground = 0x4;
  static const int nonInteractive = 0x10;
}

/// Helper class to retrieve path of the icon asset.
class ThumbnailToolbarAssetIcon {
  /// Asset location of the `*.ico` file.
  final String asset;

  String get path => join(
        dirname(Platform.resolvedExecutable),
        'data',
        'flutter_assets',
        asset,
      );

  ThumbnailToolbarAssetIcon(this.asset) {
    assert(
      Platform.isWindows && asset.endsWith('.ico') && File(path).existsSync(),
    );
  }
}

/// Represents a thumbnail toolbar button.
///
class ThumbnailToolbarButton {
  /// [File] to the icon of the button. Must be `*.ico` format.
  final ThumbnailToolbarAssetIcon icon;

  /// Tooltip of the button. Showed upon hover.
  final String tooltip;

  /// Display configuration of the button. See [ThumbnailToolbarButtonMode] for more information.
  final int mode;

  /// Called when button is clicked from the toolbar.
  final void Function() onClick;

  ThumbnailToolbarButton(
    this.icon,
    this.tooltip,
    this.onClick, {
    this.mode = 0x0,
  });

  /// Conversion to `flutter::EncodableMap` for method channel transfer.
  Map<String, dynamic> toJson() => {
        'icon': icon.path,
        'tooltip': tooltip,
        'mode': mode,
      };
}

/// WindowsTaskbar
/// --------------
///
/// Flutter plugin serving utilities related to Windows taskbar.
///
class WindowsTaskbar {
  /// Sets thumbnail toolbar for the taskbar app icon.
  /// Takes list of thumbnail toolbar buttons.
  ///
  static Future<void> setThumbnailToolbar(
      List<ThumbnailToolbarButton> buttons) {
    assert(buttons.length <= _kMaximumButtonCount);
    _buttons = buttons;
    return _kChannel.invokeMethod(
      _kSetThumbnailToolbar,
      {
        'buttons': buttons.map((button) {
          return button.toJson();
        }).toList(),
      },
    );
  }

  /// Removes thumbnail toolbar for the taskbar app icon.
  static Future<void> clearThumbnailToolbar() {
    _buttons = [];
    return _kChannel.invokeMethod(
      _kSetThumbnailToolbar,
      {
        'buttons': [],
      },
    );
  }
}

/// Maximum button count in the thumbnail toolbar.
const int _kMaximumButtonCount = 7;

/// Last [List] of thumbnail toolbar buttons passed to [WindowsTaskbar.setThumbnailToolbar].
List<ThumbnailToolbarButton> _buttons = [];
