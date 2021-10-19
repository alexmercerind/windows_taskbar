<h1 align="center"><a href="https://github.com/alexmercerind/windows_taskbar">windows_taskbar</a></h1>
<h4 align="center">Flutter plugin serving utilities related to Windows taskbar 💙</h4>

## Install

```yaml
dependencies: 
  windows_taskbar: ^0.0.1
```

## Demo

Checkout the example Flutter application [here](https://github.com/alexmercerind/windows_taskbar/blob/master/example/lib/main.dart).

![](https://github.com/alexmercerind/windows_taskbar/blob/assets/bMebejv57F.gif)
![](https://github.com/alexmercerind/windows_taskbar/blob/assets/OdzgD1yqEO.gif)


## Documentation

**Set thumbnail toolbar**

```dart
WindowsTaskbar.setThumbnailToolbar(
  [
  ThumbnailToolbarButton(
    ThumbnailToolbarAssetIcon('res/previous.ico'),
      'Button 1',
      () {},
    ),
    ThumbnailToolbarButton(
      ThumbnailToolbarAssetIcon('res/pause.ico'),
      'Button 2',
      () {},
      mode: ThumbnailToolbarButtonMode.disabled | ThumbnailToolbarButtonMode.dismissionClick,
    ),
    ThumbnailToolbarButton(
      ThumbnailToolbarAssetIcon('res/next.ico'),
      'Button 3',
      () {},
    ),
  ],
);
```

**Clear thumbnail toolbar**

```dart
WindowsTaskbar.clearThumbnailToolbar();
```

**Set progress mode**

```dart
WindowsTaskbar.setProgressMode(TaskbarProgressMode.indeterminate);
```

**Set progress**

```dart
WindowsTaskbar.setProgress(69, 100);
```

**Set thumbnail tooltip**

```dart
WindowsTaskbar.setThumbnailTooltip('Awesome Flutter window.);
```

## License

GNU Lesser General Public License v2.1

Copyright © 2021, Hitesh Kumar Saini <<saini123hitesh@gmail.com>>

Contributions welcomed.

