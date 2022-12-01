<h1 align="center"><a href="https://github.com/alexmercerind/windows_taskbar">windows_taskbar</a></h1>
<p align="center">Flutter plugin serving utilities related to Windows taskbar.</p>

## Install

```yaml
dependencies:
  windows_taskbar: ^1.1.1
```

<table>
  <tr>
    <td>
      <img width="713" height="auto" src="https://user-images.githubusercontent.com/28951144/205107667-c3db0382-a5fa-456d-af01-7c5ab9c94ceb.gif">
    </td>
  </tr>
  <tr>
    <td>
      <img width="713" height="auto" src="https://user-images.githubusercontent.com/28951144/205107645-9732aabe-0e39-42c7-87d3-bbd2bca82fa6.gif">
    </td>
  </tr>
</table>

Checkout the example Flutter application [here](https://github.com/alexmercerind/windows_taskbar/blob/master/example/lib/main.dart).

## Docs

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

#### Set overlay icon (a badge)

```dart
WindowsTaskbar.setOverlayIcon(
  ThumbnailToolbarAssetIcon('assets/red_slash.ico'),
  tooltip: 'Stop',
);
```

#### Remove overlay icon (a badge)

```dart
WindowsTaskbar.resetOverlayIcon();
```

#### Set window title (or taskbar button label)

```dart
WindowsTaskbar.setWindowTitle('Never Gonna Give You Up');
```

#### Reset window title (or taskbar button label)

```dart
WindowsTaskbar.resetWindowTitle();
```

Please see [API reference](https://pub.dev/documentation/windows_taskbar/latest/windows_taskbar/windows_taskbar-library.html) for extended information & more detail about various configurations.

## Sponsor

You may consider buying me a coffee if you like this package.

- [GitHub Sponsors](https://github.com/sponsors/alexmercerind)
- [PayPal](https://paypal.me/alexmercerind)

## License

Copyright © 2021 & onwards, Hitesh Kumar Saini <<saini123hitesh@gmail.com>>

This library & all the work in this repository is under MIT license that can be found in the LICENSE file.
