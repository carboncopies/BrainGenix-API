# BrainGenix-API Mac Silicon Change Log

Reviewed on 2026-05-09 against refreshed `origin/master`.

## Current branch

- Branch: `feature/mac-silicon-support`
- Compared base: `origin/master` at `55a77d78`
- Upstream integration: rebased `feature/mac-silicon-support` onto refreshed `origin/master`.
- Local status during review: `Tools/Build.sh` and `Tools/Setup.sh` modified; `api_build.log` untracked and should not be committed.

## Intent

Allow the same contributor workflow used on the Linux VM to work on Apple Silicon macOS:

```bash
cd BrainGenix-API/Tools
./Setup.sh
./Build.sh 6 Release
./Run.sh
```

Linux should keep the existing apt/g++/default-CMake behavior. macOS should detect Darwin and use Homebrew, Ninja, clang++, and the active Xcode SDK automatically.

## Changes made

- `CMakeLists.txt`
  - Selects `.dylib` for Python linking on macOS and `.so` elsewhere.
  - This is platform-specific and appropriate.

- `Tools/Setup.sh`
  - Keeps the original Linux dependency path.
  - Adds a Darwin path that installs Homebrew dependencies and creates the project `venv`.
  - Installs `neuroglancer` into the venv on both platforms.

- `Tools/Build.sh`
  - Parses both legacy positional usage (`./Build.sh 60 Release`) and explicit jobs usage (`./Build.sh --jobs 6 Release`).
  - Uses `sysctl -n hw.ncpu` on macOS and `nproc` on Linux when no job count is provided.
  - Uses Ninja, clang++, Xcode SDK, and the CMake policy workaround only on macOS.
  - Preserves the original generic `cmake .. -D CMAKE_BUILD_TYPE=...` configure path on Linux.

## Review notes

- The first macOS version of `Tools/Build.sh` always called `xcrun`, `clang++`, and Ninja. That would break the Linux VM path. This was narrowed to Darwin only.
- The current changes are small and merge-friendly: build script detection, dependency setup, and Python library suffix selection.
- Do not commit `api_build.log`; it is local build output.

## Verification

- `bash -n` passes for `Tools/Build.sh` and `Tools/Setup.sh`.
- Previous integrated run built and launched API successfully on Apple Silicon.
- After the user's successful terminal pull, local `origin/master` was refreshed and the feature branch rebased cleanly.
- After the refreshed branch update, API rebuilt successfully with `./Build.sh 6 Release` and participated in the local `xor_scnm` integrated run through `OK Freeing Voxel Array`.
