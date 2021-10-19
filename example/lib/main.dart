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
  String? lastTooltip;
  String? lastIcon;

  @override
  void initState() {
    super.initState();
    enableThumbnailToolbar();
  }

  void enableThumbnailToolbar() {
    WindowsTaskbar.setThumbnailToolbar(
      [
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('res/folder.ico'),
          'Button 1',
          () {
            setState(() {
              lastTooltip = 'Button 1';
              lastIcon = 'res/folder.ico';
            });
          },
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('res/pictures.ico'),
          'Button 2',
          () {
            setState(() {
              lastTooltip = 'Button 2';
              lastIcon = 'res/pictures.ico';
            });
          },
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('res/sync.ico'),
          'Button 3',
          () {
            setState(() {
              lastTooltip = 'Button 3';
              lastIcon = 'res/sync.ico';
            });
          },
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('res/music.ico'),
          'Button 4',
          () {
            setState(() {
              lastTooltip = 'Button 4';
              lastIcon = 'res/music.ico';
            });
          },
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
        body: ListView(
          padding: const EdgeInsets.symmetric(vertical: 4.0),
          children: [
            Card(
              elevation: 2.0,
              margin: const EdgeInsets.symmetric(
                horizontal: 16.0,
                vertical: 8.0,
              ),
              child: Padding(
                padding: const EdgeInsets.all(12.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'WindowsTaskbar.setThumbnailToolbar',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    const Text(
                      'Sets thumbnail toolbar for the taskbar app icon.',
                    ),
                    const SizedBox(
                      height: 12.0,
                    ),
                    ElevatedButton(
                      onPressed: enableThumbnailToolbar,
                      child: const Text('Enable'),
                    ),
                    Container(
                      height: 32.0,
                      alignment: Alignment.bottomLeft,
                      child: Row(
                        children: [
                          const Text('Pressed button:'),
                          Text('$lastTooltip'),
                          if (lastIcon != null)
                            Image.asset(
                              lastIcon!,
                              height: 18.0,
                              width: 18.0,
                            ),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
            ),
            Card(
              elevation: 2.0,
              margin: const EdgeInsets.symmetric(
                horizontal: 16.0,
                vertical: 8.0,
              ),
              child: Padding(
                padding: const EdgeInsets.all(12.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: const [
                    Text(
                      'WindowsTaskbar.clearThumbnailToolbar',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    Text(
                      'Removes thumbnail toolbar for the taskbar app icon.',
                    ),
                    SizedBox(
                      height: 12.0,
                    ),
                    ElevatedButton(
                      onPressed: WindowsTaskbar.clearThumbnailToolbar,
                      child: Text('Clear'),
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
