# Licensed Asset Loader (LAL)

This directory contains the implementation of the Licensed Asset Loader, a system for managing proprietary firmware, drivers, and game support files for Linuxtrack.

## Structure

*   **`lal_manager.h/cpp`**: Core C++ logic for managing assets.
*   **`lal_manifest.json`**: The database of known assets, their sources, SHA-256 hashes, and extraction rules.

## Design Goals

1.  **Vendor Neutrality**: Supports assets from NaturalPoint, Tobii, etc.
2.  **Safety**: Validates hashes before installation.
3.  **No Wine Runtime**: Uses native Linux tools (`7z`, `tar`) for extraction whenever possible.
4.  **Separation**: Keeps open-source code separate from proprietary binaries.

## Usage (Future)

The `LALManager` class will be integrated into `ltr_gui` to provide an "Asset Manager" UI.
