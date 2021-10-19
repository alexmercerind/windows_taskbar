import 'package:flutter/material.dart';
import 'package:windows_taskbar/windows_taskbar.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  void initState() {
    super.initState();
    WindowsTaskbar.setThumbnailToolbar(
      [
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('res/folder.ico'),
          'Button 1',
          (button) {},
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('res/pictures.ico'),
          'Button 2',
          (button) {},
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('res/sync.ico'),
          'Button 3',
          (button) {},
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('windows_taskbar'),
        ),
        body: Center(),
      ),
    );
  }
}
