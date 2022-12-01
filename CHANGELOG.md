## 1.1.1

- feat: `WindowsTaskbar.setWindowTitle` & `WindowsTaskbar.resetWindowTitle` (@alexmercerind).
- refactor: check `nullptr` before invoking `ITaskbarList3` methods (@alexmercerind).

## 1.1.0

- feat: `WindowsTaskbar.setOverlayIcon` & `WindowsTaskbar.resetOverlayIcon` (@nikitatg).
- feat: platform channel calls raise errors correctly by checking `HRESULT` (@alexmercerind).
- perf: move impl to separate translation unit & make methods non-inline (@alexmercerind).
- refactor: improved naming of various methods (@alexmercerind).
- deprecated:
  - `WindowsTaskbar.clearThumbnailToolbar` is now `WindowsTaskbar.resetThumbnailToolbar`.
  - `WindowsTaskbar.setFlashTaskbarAppIcon` is now `WindowsTaskbar.setFlashTaskbarAppIcon`.
  - `WindowsTaskbar.stopFlashingTaskbarAppIcon` is now `WindowsTaskbar.resetFlashTaskbarAppIcon`.

## 1.0.0

- feat: `WindowsTaskbar.flashTaskbarAppIcon` & `WindowsTaskbar.stopFlashingTaskbarAppIcon` (@alexmercerind).
- fix: use `MultiByteToWideChar` for `std::wstring` convert (@boyan01).
- chore: re-license to MIT (@alexmercerind).

## 0.0.1

- feat: most of the common `ITaskbarList3` Win32 APIs (@alexmercerind).
- feat: `SetProgressMode` (@alexmercerind).
- feat: `SetProgress` (@alexmercerind).
- feat: `SetThumbnailToolbar` (@alexmercerind).
- feat: `SetThumbnailTooltip` (@alexmercerind).
