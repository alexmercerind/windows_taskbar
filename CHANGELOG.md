## 1.0.0

- Implement `WindowsTaskbar.flashTaskbarAppIcon` & `WindowsTaskbar.stopFlashingTaskbarAppIcon` (@alexmercerind).
- Change project license to MIT (@alexmercerind).
- Use `MultiByteToWideChar` for `std::wstring` convert (@boyan01).

## 0.0.1

- Initial release.
- Expose all common `ITaskbarList3` WIN32 APIs (@alexmercerind).
- Add following methods to `WindowsTaskbar` (@alexmercerind):
  - `SetProgressMode`
  - `SetProgress`
  - `SetThumbnailToolbar`
  - `SetThumbnailTooltip`
