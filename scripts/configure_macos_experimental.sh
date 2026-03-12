#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"

build_dir="${1:-${repo_root}/build_macos_experimental}"
install_dir="${2:-${repo_root}/stage_macos_experimental}"

if [[ $# -ge 1 ]]; then
  shift
fi
if [[ $# -ge 1 ]]; then
  shift
fi

cmake -S "${repo_root}" -B "${build_dir}" \
  -DBUILD_MAC_EXPERIMENTAL=ON \
  -DBUILD_MICKEY=OFF \
  -DBUILD_WINE_BRIDGE=OFF \
  -DBUILD_NEURALNET_TRACKER=OFF \
  -DCMAKE_INSTALL_PREFIX="${install_dir}" \
  "$@"
