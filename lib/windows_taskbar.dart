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

/// Sets progress mode.
const String _kSetProgressMode = 'SetProgressMode';

/// Sets progress.
const String _kSetProgress = 'SetProgress';

/// Sets thumbnail toolbar.
const String _kSetThumbnailToolbar = 'SetThumbnailToolbar';

/// Sets thumbnail tooltip.
const String _kSetThumbnailTooltip = 'SetThumbnailTooltip';

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
  /// Disabled thumbnail toolbar button.
  static const int disabled = 0x1;

  /// When enabled, the thumbnail is dismissed upon click.
  static const int dismissionClick = 0x2;

  /// Do not draw a button border, use only the image.
  static const int noBackground = 0x4;

  /// The button is enabled but not interactive; no pressed button state is drawn.
  /// This value is intended for instances where the button is used in a notification.
  ///
  static const int nonInteractive = 0x10;
}

/// Taskbar progress mode.
class TaskbarProgressMode {
  /// No progress state of taskbar app icon.
  static const int noProgress = 0x0;

  /// Indeterminate progress state of taskbar app icon.
  static const int indeterminate = 0x1;

  /// Normal progress state of taskbar app icon.
  static const int normal = 0x2;

  /// Errored progress state of taskbar app icon.
  static const int error = 0x4;

  /// Paused progress state of taskbar app icon.
  static const int paused = 0x8;
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
  /// Sets progress mode.
  ///
  /// ```dart
  /// WindowsTaskbar.setProgressMode(TaskbarProgressMode.indeterminate);
  /// ```
  ///
  static Future<void> setProgressMode(int mode) {
    return _kChannel.invokeMethod(
      _kSetProgressMode,
      {
        'mode': mode,
      },
    );
  }

  /// Sets progress.
  ///
  /// ```dart
  /// WindowsTaskbar.setProgress(69, 100);
  /// ```
  ///
  static Future<void> setProgress(int completed, int total) {
    return _kChannel.invokeMethod(
      _kSetProgress,
      {
        'completed': completed,
        'total': total,
      },
    );
  }

  /// Sets thumbnail toolbar for the taskbar app icon.
  /// Takes list of thumbnail toolbar buttons.
  ///
  /// ```dart
  /// WindowsTaskbar.setThumbnailToolbar(
  ///   [
  ///     ThumbnailToolbarButton(
  ///       ThumbnailToolbarAssetIcon('res/previous.ico'),
  ///         'Button 1',
  ///         () {},
  ///       ),
  ///       ThumbnailToolbarButton(
  ///         ThumbnailToolbarAssetIcon('res/pause.ico'),
  ///         'Button 2',
  ///         () {},
  ///         mode: ThumbnailToolbarButtonMode.disabled | ThumbnailToolbarButtonMode.dismissionClick,
  ///      ),
  ///      ThumbnailToolbarButton(
  ///        ThumbnailToolbarAssetIcon('res/next.ico'),
  ///        'Button 3',
  ///        () {},
  ///      ),
  ///    ],
  ///  );
  /// ```
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

  /// Sets thumbnail tooltip.
  ///
  /// ```dart
  /// WindowsTaskbar.setThumbnailTooltip('An awesome Flutter window.');
  /// ```
  ///
  static Future<void> setThumbnailTooltip(String tooltip) {
    return _kChannel.invokeMethod(
      _kSetThumbnailTooltip,
      {
        'tooltip': tooltip,
      },
    );
  }
}

/// Maximum button count in the thumbnail toolbar.
const int _kMaximumButtonCount = 7;

/// Last [List] of thumbnail toolbar buttons passed to [WindowsTaskbar.setThumbnailToolbar].
List<ThumbnailToolbarButton> _buttons = [];
