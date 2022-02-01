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
  double value = 0.0;

  @override
  void initState() {
    super.initState();
    enableThumbnailToolbar();
    WindowsTaskbar.setThumbnailTooltip('Awesome Flutter window.');
    WindowsTaskbar.setProgress(69, 100);
    WindowsTaskbar.setProgressMode(TaskbarProgressMode.indeterminate);
  }

  void enableThumbnailToolbar() {
    WindowsTaskbar.setThumbnailToolbar(
      [
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/folder.ico'),
          'Button 1',
          () {
            setState(() {
              lastTooltip = 'Button 1';
              lastIcon = 'assets/folder.ico';
            });
          },
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/pictures.ico'),
          'Button 2',
          () {
            setState(() {
              lastTooltip = 'Button 2';
              lastIcon = 'assets/pictures.ico';
            });
          },
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/sync.ico'),
          'Button 3',
          () {
            setState(() {
              lastTooltip = 'Button 3';
              lastIcon = 'assets/sync.ico';
            });
          },
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/music.ico'),
          'Button 4',
          () {
            setState(() {
              lastTooltip = 'Button 4';
              lastIcon = 'assets/music.ico';
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
                      'WindowsTaskbar.resetThumbnailToolbar',
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
                      onPressed: WindowsTaskbar.resetThumbnailToolbar,
                      child: Text('Reset'),
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
                  children: [
                    const Text(
                      'WindowsTaskbar.setProgressMode',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    const Text(
                      'Sets progress mode.',
                    ),
                    const SizedBox(
                      height: 12.0,
                    ),
                    Row(
                      children: [
                        ElevatedButton(
                          onPressed: () {
                            WindowsTaskbar.setProgressMode(
                                TaskbarProgressMode.error);
                          },
                          child: const Text('error'),
                        ),
                        const SizedBox(
                          width: 4.0,
                        ),
                        ElevatedButton(
                          onPressed: () {
                            WindowsTaskbar.setProgressMode(
                                TaskbarProgressMode.indeterminate);
                          },
                          child: const Text('indeterminate'),
                        ),
                        const SizedBox(
                          width: 4.0,
                        ),
                        ElevatedButton(
                          onPressed: () {
                            WindowsTaskbar.setProgressMode(
                                TaskbarProgressMode.noProgress);
                          },
                          child: const Text('noProgress'),
                        ),
                        const SizedBox(
                          width: 4.0,
                        ),
                        ElevatedButton(
                          onPressed: () {
                            WindowsTaskbar.setProgressMode(
                                TaskbarProgressMode.normal);
                          },
                          child: const Text('normal'),
                        ),
                        const SizedBox(
                          width: 4.0,
                        ),
                        ElevatedButton(
                          onPressed: () {
                            WindowsTaskbar.setProgressMode(
                                TaskbarProgressMode.paused);
                          },
                          child: const Text('paused'),
                        ),
                      ],
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
                  children: [
                    const Text(
                      'WindowsTaskbar.setProgress',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    const Text(
                      'Sets progress.',
                    ),
                    const SizedBox(
                      height: 12.0,
                    ),
                    Row(
                      children: [
                        Slider(
                          value: value,
                          min: 0.0,
                          max: 1.0,
                          onChanged: (v) {
                            WindowsTaskbar.setProgress(v * 100 ~/ 1, 100);
                            setState(() {
                              value = v;
                            });
                          },
                        )
                      ],
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
                  children: [
                    const Text(
                      'WindowsTaskbar.setThumbnailTooltip',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    const Text(
                      'Sets thumbnail tooltip.',
                    ),
                    const SizedBox(
                      height: 12.0,
                    ),
                    TextField(
                      decoration: const InputDecoration(
                        label: Text('Enter a tooltip'),
                      ),
                      onChanged: (value) {
                        WindowsTaskbar.setThumbnailTooltip(value);
                      },
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
                  children: [
                    const Text(
                      'WindowsTaskbar.setFlashTaskbarAppIcon',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    const Text(
                      'Flashes app icon on the taskbar.\nGenerally used to draw user attention when something needs to be approved/rejected or fixed manually.\n\nA lot of options are available to configure the behaviour of this method.',
                    ),
                    const SizedBox(
                      height: 12.0,
                    ),
                    ElevatedButton(
                      onPressed: () => WindowsTaskbar.setFlashTaskbarAppIcon(
                        mode: TaskbarFlashMode.all,
                        flashCount: 500,
                        timeout: const Duration(milliseconds: 100),
                      ),
                      child: const Text('Start Flashing'),
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
                      'WindowsTaskbar.resetFlashTaskbarAppIcon',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    Text(
                      'Stops flashing the taskbar app icon.\nUndoes the results achieved by [WindowsTaskbar.setFlashTaskbarAppIcon].',
                    ),
                    SizedBox(
                      height: 12.0,
                    ),
                    ElevatedButton(
                      onPressed: WindowsTaskbar.resetFlashTaskbarAppIcon,
                      child: Text('Stop Flashing'),
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
                  children: [
                    const Text(
                      'WindowsTaskbar.setOverlayIcon',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    const Text(
                      'Set icon on top of taskbar app icon.',
                    ),
                    const SizedBox(
                      height: 12.0,
                    ),
                    ElevatedButton(
                      onPressed: () => WindowsTaskbar.setOverlayIcon(
                        ThumbnailToolbarAssetIcon('assets/red_slash.ico'),
                        tooltip: 'Stop',
                      ),
                      child: const Text('Set overlay icon'),
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
                  children: [
                    const Text(
                      'WindowsTaskbar.resetOverlayIcon',
                      style: TextStyle(
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                    const Text(
                      'Resets (hides) overlay icon that set by [WindowsTaskbar.setOverlayIcon].',
                    ),
                    const SizedBox(
                      height: 12.0,
                    ),
                    ElevatedButton(
                      onPressed: () => WindowsTaskbar.resetOverlayIcon(),
                      child: const Text('Reset overlay icon'),
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
