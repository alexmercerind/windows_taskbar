# [windows_taskbar](https://github.com/alexmercerind/windows_taskbar)

#### Flutter plugin serving utilities related to Windows taskbar.

## Installation

Add package to the dependencies section of the `pubspec.yaml`:

```yaml
dependencies:
  windows_taskbar: ^1.1.2
```

<br></br>

<img width="600" height="auto" src="https://user-images.githubusercontent.com/28951144/205107667-c3db0382-a5fa-456d-af01-7c5ab9c94ceb.gif">
<img width="600" height="auto" src="https://user-images.githubusercontent.com/28951144/205107645-9732aabe-0e39-42c7-87d3-bbd2bca82fa6.gif">

## Guide

A minimal usage guide for [package:windows_taskbar](https://github.com/alexmercerind/windows_taskbar).

### Contents

- [Set thumbnail toolbar](#set-thumbnail-toolbar)
- [Remove thumbnail toolbar](#remove-thumbnail-toolbar)
- [Set progress mode](#set-progress-mode)
- [Set progress](#set-progress)
- [Set thumbnail tooltip](#set-thumbnail-tooltip)
- [Flash windows taskbar app icon](#flash-windows-taskbar-app-icon)
- [Stop flashing windows taskbar app icon](#stop-flashing-windows-taskbar-app-icon)
- [Set overlay icon (badge)](#set-overlay-icon-badge)
- [Remove overlay icon (badge)](#remove-overlay-icon-badge)
- [Set window title (taskbar button label)](#set-window-title-taskbar-button-label)
- [Reset window title (or taskbar button label)](#reset-window-title-taskbar-button-label)
- [Next steps](#next-steps)

#### Set thumbnail toolbar

```dart
WindowsTaskbar.setThumbnailToolbar(
  [
  ThumbnailToolbarButton(
    ThumbnailToolbarAssetIcon('assets/camera.ico'),
      'Turn On Camera',
      () {},
    ),
    ThumbnailToolbarButton(
      ThumbnailToolbarAssetIcon('assets/microphone.ico'),
      'Mute',
      () {},
      mode: ThumbnailToolbarButtonMode.disabled | ThumbnailToolbarButtonMode.dismissionClick,
    ),
    ThumbnailToolbarButton(
      ThumbnailToolbarAssetIcon('assets/end_call.ico'),
      'Disconnect',
      () {},
    ),
  ],
);
```

#### Remove thumbnail toolbar

```dart
WindowsTaskbar.resetThumbnailToolbar();
```

#### Set progress mode

```dart
WindowsTaskbar.setProgressMode(TaskbarProgressMode.indeterminate);
```

#### Set progress

```dart
WindowsTaskbar.setProgress(69, 100);
```

#### Set thumbnail tooltip

```dart
WindowsTaskbar.setThumbnailTooltip('Awesome Flutter window.');
```

#### Flash windows taskbar app icon

```dart
WindowsTaskbar.setFlashTaskbarAppIcon(
  mode: TaskbarFlashMode.all | TaskbarFlashMode.timernofg,
  timeout: const Duration(milliseconds: 500),
);
```

#### Stop flashing windows taskbar app icon

```dart
WindowsTaskbar.resetFlashTaskbarAppIcon();
```

#### Set overlay icon (badge)

```dart
WindowsTaskbar.setOverlayIcon(
  ThumbnailToolbarAssetIcon('assets/red_slash.ico'),
  tooltip: 'Stop',
);
```

#### Remove overlay icon (badge)

```dart
WindowsTaskbar.resetOverlayIcon();
```

#### Set window title (taskbar button label)

```dart
WindowsTaskbar.setWindowTitle('Never Gonna Give You Up');
```

#### Reset window title (taskbar button label)

```dart
WindowsTaskbar.resetWindowTitle();
```

#### Next steps

- [API reference](https://pub.dev/documentation/windows_taskbar/latest/windows_taskbar/windows_taskbar-library.html) can be helpful for diving into deeper specifics.
- [Source-code of the example application](https://github.com/alexmercerind/windows_taskbar/blob/master/example/lib/main.dart) offers some complete code samples.

## Sponsor

You may consider buying me a coffee if you like this package.

- [GitHub Sponsors](https://github.com/sponsors/alexmercerind)
- [PayPal](https://paypal.me/alexmercerind)

## License

Copyright © 2021 & onwards, Hitesh Kumar Saini <<saini123hitesh@gmail.com>>

This project & the work under this repository is governed by MIT license that can be found in the [LICENSE](https://github.com/alexmercerind/windows_taskbar/blob/master/LICENSE) file.
