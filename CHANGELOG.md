## 1.1.1

- feat: `WindowsTaskbar.setWindowTitle` & `WindowsTaskbar.resetWindowTitle`
- refactor: check `nullptr` before invoking `ITaskbarList3` methods

## 1.1.0

- feat: `WindowsTaskbar.setOverlayIcon` & `WindowsTaskbar.resetOverlayIcon`
- feat: platform channel calls raise errors correctly by checking `HRESULT`
- perf: move impl to separate translation unit & make methods non-inline
- refactor: improved naming of various methods
- deprecated:
  - `WindowsTaskbar.clearThumbnailToolbar` is now `WindowsTaskbar.resetThumbnailToolbar`.
  - `WindowsTaskbar.setFlashTaskbarAppIcon` is now `WindowsTaskbar.setFlashTaskbarAppIcon`.
  - `WindowsTaskbar.stopFlashingTaskbarAppIcon` is now `WindowsTaskbar.resetFlashTaskbarAppIcon`.

## 1.0.0

- feat: `WindowsTaskbar.flashTaskbarAppIcon` & `WindowsTaskbar.stopFlashingTaskbarAppIcon`
- fix: use `MultiByteToWideChar` for `std::wstring` convert
- chore: re-license to MIT

## 0.0.1

- feat: most of the common `ITaskbarList3` Win32 APIs
- feat: `SetProgressMode`
- feat: `SetProgress`
- feat: `SetThumbnailToolbar`
- feat: `SetThumbnailTooltip`
