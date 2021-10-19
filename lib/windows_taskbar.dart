
import 'dart:async';

import 'package:flutter/services.dart';

class WindowsTaskbar {
  static const MethodChannel _channel = MethodChannel('windows_taskbar');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
