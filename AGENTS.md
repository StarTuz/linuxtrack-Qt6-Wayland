# Repository Guidelines

## Project Structure & Module Organization
Core runtime code lives in `src/`. Key areas include `src/qt_gui/` for the Qt6 desktop app, `src/lal/` for the asset manager, `src/wine_bridge/` for Wine/Proton support, and `src/tests/` for lightweight unit tests. Packaging and distribution scripts live under `packaging/appimage/` and `scripts/`. Documentation is split across root-level notes such as `README.md`, `MODERNIZATION_ROADMAP.md`, and focused docs in `doc/`.

Keep source edits in tracked directories only. Do not commit generated content from `build*/`, `*_autogen/`, `.qt/`, or other local build outputs.

## Build, Test, and Development Commands
Use an out-of-tree build:

```bash
cmake -S . -B build
cmake --build build -j"$(nproc)"
sudo cmake --install build
```

Common targets:

- `cmake --build build --target ltr_gui` builds the main GUI quickly.
- `cmake --build build --target test_lal` builds the maintained LAL unit test.
- `cd src/tests && make test` runs the Catch2-based `modern_prefs` test runner.
- `scripts/smoke_test.sh /absolute/path/Linuxtrack.AppImage` runs multi-distro AppImage smoke checks in Docker.

## Coding Style & Naming Conventions
Follow the existing C/C++ style: 2-space indentation, opening braces on the same line, and minimal inline comments. Use `snake_case` for C functions/files such as `ltr_hotkeyd.c`, and preserve established Qt/C++ naming where classes use `PascalCase` and methods use lowerCamelCase, for example `LALManager::loadManifest`.

`scripts/pre-commit` supports optional `clang-format` and `cppcheck`; use them when available, but match surrounding code before applying broad formatting changes.

## Testing Guidelines
Add or update tests for new logic when practical. Prefer extending `src/lal/test_lal.cpp` for LAL changes and `src/tests/test_modern_prefs.cpp` for config parsing work. Name new tests after the feature under test, and keep them deterministic so they can run locally without hardware.

## Commit & Pull Request Guidelines
Recent history uses short, imperative subjects with optional scope prefixes, for example `Qt GUI: Remove WA_NativeWindow...`, `CI: Fix Arch Linux build`, and `Update smoke tests for missing dependencies`. Keep commits focused and describe the user-visible effect.

PRs should include a concise summary, affected platforms or hardware paths, test/build commands run, and screenshots for GUI changes. Call out any dependency, udev, Wine/Proton, or packaging impact explicitly.
