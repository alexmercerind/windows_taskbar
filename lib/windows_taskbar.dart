/// This file is a part of windows_taskbar (https://github.com/alexmercerind/windows_taskbar).
///
/// Copyright (c) 2021 & onwards, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
/// All rights reserved.
/// Use of this source code is governed by MIT license that can be found in the LICENSE file.

import 'dart:io';

import 'package:flutter/services.dart';
import 'package:path/path.dart';

/// Sets progress mode.
const String _kSetProgressMode = 'SetProgressMode';

/// Sets progress.
const String _kSetProgress = 'SetProgress';

/// Sets thumbnail toolbar.
const String _kSetThumbnailToolbar = 'SetThumbnailToolbar';

/// Removes thumbnail toolbar.
const String _kResetThumbnailToolbar = 'ResetThumbnailToolbar';

/// Sets thumbnail tooltip.
const String _kSetThumbnailTooltip = 'SetThumbnailTooltip';

/// Flashes app icon on the taskbar.
const String _kSetFlashTaskbarAppIcon = 'SetFlashTaskbarAppIcon';

/// Stops flashing the taskbar app icon.
const String _kResetFlashTaskbarAppIcon = 'ResetFlashTaskbarAppIcon';

/// Sets overlay icon (a badge).
const String _kSetOverlayIcon = 'SetOverlayIcon';

/// Removes overlay icon (a badge).
const String _kResetOverlayIcon = 'ResetOverlayIcon';

/// Set window title.
const String _kSetWindowTitle = 'SetWindowTitle';

/// Reset window title.
const String _kResetWindowTitle = 'ResetWindowTitle';

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

/// Determines how taskbar app icon should be flashed.
class TaskbarFlashMode {
  /// Stop flashing. The system restores the window to its original state.
  static const int stop = 0;

  /// Flash the window caption.
  static const int caption = 1;

  /// Flash the taskbar button.
  static const int tray = 2;

  /// Flash both the window caption and taskbar button.
  /// This is equivalent to setting the `caption | tray` flags.
  static const int all = 3;

  /// Flash continuously, until the `stop` flag is set.
  static const int timer = 4;

  /// Flash continuously until the window comes to the foreground.
  static const int timernofg = 12;
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
  static Future<void> resetThumbnailToolbar() {
    _buttons = [];
    return _kChannel.invokeMethod(
      _kResetThumbnailToolbar,
      {},
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

  /// Flashes app icon on the taskbar.
  /// Generally used to draw user attention when something needs to be approved/rejected or fixed manually.
  ///
  /// * [mode] determines how the taskbar app icon should be flashed. See [TaskbarFlashMode] for more details.
  ///
  /// * [flashCount] sets how many times the taskbar app icon should be flashed.
  ///
  /// * [timeout] sets the interval timeout between each flash. If passed as `0`, it uses default cursor blink rate.
  ///
  static Future<void> setFlashTaskbarAppIcon({
    int mode = TaskbarFlashMode.all | TaskbarFlashMode.timernofg,
    int flashCount = 5,
    Duration timeout = Duration.zero,
  }) {
    assert(flashCount <= 2147483647);
    return _kChannel.invokeMethod(
      _kSetFlashTaskbarAppIcon,
      {
        'mode': mode,
        'flashCount': flashCount,
        'timeout': timeout.inMilliseconds,
      },
    );
  }

  /// Stops flashing the taskbar app icon.
  ///
  /// Undoes the results achieved by [WindowsTaskbar.setFlashTaskbarAppIcon].
  ///
  static Future<void> resetFlashTaskbarAppIcon() {
    return _kChannel.invokeMethod(
      _kResetFlashTaskbarAppIcon,
      {},
    );
  }

  /// Set icon on top of taskbar app icon
  /// Generally used to draw user attention when got unread events
  ///
  /// * [icon] takes [ThumbnailToolbarAssetIcon] & sets it as overlay icon.
  ///
  /// * [tooltip] sets system tooltip to overlay on the icon.
  ///
  static Future<void> setOverlayIcon(
    ThumbnailToolbarAssetIcon icon, {
    String tooltip = '',
  }) {
    return _kChannel.invokeMethod(
      _kSetOverlayIcon,
      {
        'icon': icon.path,
        'tooltip': tooltip,
      },
    );
  }

  /// Resets (hides) overlay icon that set by [setOverlayIcon]
  ///
  /// Generally used to indicate current app state e.g. count of unread messages.
  ///
  static Future<void> resetOverlayIcon() {
    return _kChannel.invokeMethod(
      _kResetOverlayIcon,
      {},
    );
  }

  /// Resets (hides) overlay icon that set by [setOverlayIcon]
  ///
  /// Generally used to indicate current app state e.g. count of unread messages.
  ///
  static Future<void> setWindowTitle(String title) {
    return _kChannel.invokeMethod(
      _kSetWindowTitle,
      {
        'title': title,
      },
    );
  }

  /// Resets (hides) overlay icon that set by [setOverlayIcon]
  ///
  /// Generally used to indicate current app state e.g. count of unread messages.
  ///
  static Future<void> resetWindowTitle() {
    return _kChannel.invokeMethod(
      _kResetWindowTitle,
      {},
    );
  }

  /// Deprecated API present to avoid breaking changes.
  @Deprecated('Use [WindowsTaskbar.resetThumbnailToolbar] instead.')
  static Future<void> clearThumbnailToolbar() =>
      WindowsTaskbar.resetThumbnailToolbar();

  /// Deprecated API present to avoid breaking changes.
  @Deprecated('Use [WindowsTaskbar.resetThumbnailToolbar] instead.')
  static Future<void> flashTaskbarAppIcon({
    int mode = TaskbarFlashMode.all | TaskbarFlashMode.timernofg,
    int flashCount = 5,
    Duration timeout = Duration.zero,
  }) =>
      WindowsTaskbar.setFlashTaskbarAppIcon(
        mode: mode,
        flashCount: flashCount,
        timeout: timeout,
      );

  /// Deprecated API present to avoid breaking changes.
  @Deprecated('Use [WindowsTaskbar.resetThumbnailToolbar] instead.')
  static Future<void> stopFlashingTaskbarAppIcon() =>
      WindowsTaskbar.resetFlashTaskbarAppIcon();
}

/// Last [List] of thumbnail toolbar buttons passed to [WindowsTaskbar.setThumbnailToolbar].
List<ThumbnailToolbarButton> _buttons = [];
