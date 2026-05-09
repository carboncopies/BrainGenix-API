# Apple Silicon macOS

BrainGenix-API supports Apple Silicon macOS through the same contributor workflow used on Linux.

## Prerequisites

- Apple Silicon Mac
- Xcode Command Line Tools
- Homebrew

Install Xcode Command Line Tools if needed:

```bash
xcode-select --install
```

Install Homebrew dependencies through the repo setup script:

```bash
cd Tools
./Setup.sh
```

## Build and run

Use the normal commands:

```bash
cd Tools
./Build.sh 6 Release
./Run.sh
```

The macOS build path automatically uses Ninja, clang++, and the active Xcode SDK. Linux builds keep the existing CMake path.

## Generated files

Build directories, binaries, virtual environments, and local logs are ignored by git. Keep generated files such as `api_build.log` out of merge requests.
