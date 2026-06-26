#!/bin/bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
VENV_DIR="$REPO_ROOT/venv"

run_cmd() {
    echo "Running: $*"
    "$@"
}

ensure_sudo() {
    if [ "${EUID}" -eq 0 ]; then
        return 0
    fi

    if command -v sudo >/dev/null 2>&1; then
        sudo -v
        return 0
    fi

    echo "This setup path needs elevated package installation privileges, but sudo is unavailable."
    echo "Install the listed packages manually and rerun the script."
    exit 1
}

install_linux_packages() {
    local distro
    distro="$(. /etc/os-release && printf '%s' "${ID:-}")"

    ensure_sudo

    case "$distro" in
        ubuntu|debian)
            run_cmd sudo apt update
            run_cmd sudo apt install -y \
                git wget cmake g++ ninja-build \
                binutils-dev libunwind-dev libdwarf-dev libdw-dev \
                curl zip unzip tar pkg-config autoconf flex bison \
                python3 python3-pip python3-venv
            ;;
        fedora)
            run_cmd sudo dnf install -y \
                git wget cmake gcc-c++ ninja-build \
                binutils-devel libunwind-devel elfutils-devel \
                curl zip unzip tar pkgconf-pkg-config autoconf flex bison \
                python3 python3-pip
            ;;
        arch)
            run_cmd sudo pacman -Sy --noconfirm \
                git wget cmake gcc ninja \
                curl zip unzip tar pkgconf autoconf flex bison \
                python
            ;;
        *)
            echo "Unsupported Linux distribution: ${distro:-unknown}"
            echo "Install compiler, Python venv, and vcpkg prerequisite packages manually, then rerun."
            exit 1
            ;;
    esac
}

install_macos_packages() {
    if ! command -v brew >/dev/null 2>&1; then
        echo "Homebrew is required on macOS. Install it from https://brew.sh/ and rerun this script."
        exit 1
    fi

    run_cmd brew install cmake git wget pkg-config openssl python@3 ninja
}

setup_venv() {
    if [ ! -d "$VENV_DIR" ]; then
        echo "Creating Python virtual environment at $VENV_DIR"
        run_cmd python3 -m venv "$VENV_DIR"
    fi

    run_cmd "$VENV_DIR/bin/python" -m pip install --upgrade pip
    run_cmd "$VENV_DIR/bin/python" -m pip install neuroglancer graphifyy
}

echo "Entering repository root: $REPO_ROOT"
cd "$REPO_ROOT"

if [ "$(uname)" = "Darwin" ]; then
    echo "Detected macOS, installing dependencies via Homebrew"
    install_macos_packages
else
    echo "Detected Linux, installing dependencies via system package manager"
    install_linux_packages
fi

setup_venv

echo "Updating submodules"
run_cmd git submodule update --init --recursive

echo "Bootstrapping vcpkg"
run_cmd "$REPO_ROOT/ThirdParty/vcpkg/bootstrap-vcpkg.sh" -disableMetrics

echo "Done, you can now run Build.sh"
