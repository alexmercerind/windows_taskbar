## 1.1.0+1

- Add documentation website.

## 1.1.0

- Added `WindowsTaskbar.setOverlayIcon` & `WindowsTaskbar.resetOverlayIcon` (@nikitatg).
- Move implementation to separate translation unit & make methods non-inline (@alexmercerind).
- Now platform channel calls raise errors correctly by checking `HRESULT` (@alexmercerind).
- Deprecated methods:
  - `WindowsTaskbar.clearThumbnailToolbar` is now `WindowsTaskbar.resetThumbnailToolbar`.
  - `WindowsTaskbar.setFlashTaskbarAppIcon` is now `WindowsTaskbar.setFlashTaskbarAppIcon`.
  - `WindowsTaskbar.stopFlashingTaskbarAppIcon` is now `WindowsTaskbar.resetFlashTaskbarAppIcon`.
  - The change has been made to fix inconsistency in names.

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
