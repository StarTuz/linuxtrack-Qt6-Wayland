# Linuxtrack Installation Path Analysis

**Date:** 2025-12-25  
**Status:** Critical Documentation  
**Author:** Analysis of working vs non-working installations

---

## Executive Summary

Linuxtrack has a complex path resolution system involving **compile-time RPATH**, **compile-time macros**, and **runtime configuration**. Misalignment between these three mechanisms causes tracking to fail even when binaries appear to run correctly.

---

## The Three Path Mechanisms

### 1. RPATH/RUNPATH (Compile-Time, Baked Into Binaries)

**What it is:** The dynamic linker search path embedded in ELF binaries during linking.

**How it's set:** In `src/Makefile.am` line 19:
```makefile
LINUXLDLFAGS = '-Wl,-rpath,$(pkglibdir)'
```

Where `pkglibdir = $(libdir)/linuxtrack` and `libdir = ${exec_prefix}/lib`

**How to inspect:**
```bash
readelf -d /opt/linuxtrack/bin/ltr_server1 | grep -E "(RPATH|RUNPATH)"
# or
strings /opt/linuxtrack/bin/ltr_server1 | grep "/opt/"
```

**Example output:**
```
RUNPATH: /opt/linuxtrack-0.99.19hid/lib:/opt/linuxtrack-0.99.19hid/lib/linuxtrack
```

**Critical:** This is set at **compile time** based on the `--prefix` passed to `./configure`. Changing `--prefix` after compilation has NO effect on existing binaries. You MUST rebuild!

---

### 2. LIB_PATH Macro (Compile-Time, Baked Into Binaries)

**What it is:** A C preprocessor macro defining the library directory path.

**How it's set:** In `src/Makefile.am` line 18:
```makefile
LINUXFLAGS = '-DLIB_PATH="$(pkglibdir)/"'
```

**Used in:** `src/utils.c` line 337:
```c
char *app_path = ltr_int_my_strdup(LIB_PATH);
```

This affects how internal library loading (driver plugins, etc.) locates shared objects.

---

### 3. PREFIX in Configuration File (Runtime)

**What it is:** User-configurable prefix stored in the config file.

**Location:** `~/.config/linuxtrack/linuxtrack1.conf`

**Format:**
```ini
[Global]
Model = NP TrackClip
Prefix = "/opt/linuxtrack/bin"
```

**Used by:** `src/linuxtrack.c` function `linuxtrack_get_prefix()` (lines 296-333) and `linuxtrack_find_library()` (lines 337-386).

**Library search logic:**
```c
// From linuxtrack.c lines 373-382
prefix = linuxtrack_get_prefix();  // e.g., "/opt/linuxtrack/bin"
while(lib_locations[i] != NULL){
    name = construct_name(prefix, "/../", lib_locations[i++]);
    // Results in: /opt/linuxtrack/bin/../lib/linuxtrack/liblinuxtrack.so.0
    // Which normalizes to: /opt/linuxtrack/lib/linuxtrack/liblinuxtrack.so.0
}
```

**lib_locations array (lines 104-118):**
```c
static const char *lib_locations[] = {
    "/Frameworks/liblinuxtrack.0.dylib",
    "/lib/linuxtrack/liblinuxtrack.so.0",
    "/lib32/linuxtrack/liblinuxtrack32.so.0",
    // ... more paths
    NULL
};
```

---

## The Current Working Installation Structure

The working linuxtrack installation on this system uses a **dual-directory structure**:

### Directory Layout:
```
/opt/linuxtrack/                          # "Primary" installation
├── bin/
│   ├── ltr_server1                       # Binaries installed here
│   ├── ltr_gui
│   └── ...
├── lib/
│   └── linuxtrack/                       # Libraries (may be copies or symlinks)
│       ├── liblinuxtrack.so.0
│       ├── libltr.so.0
│       └── ...
└── share/
    └── linuxtrack/

/opt/linuxtrack-0.99.19hid/               # "Library" installation  
├── bin/                                   # Original binaries (may be duplicates)
├── lib/
│   └── linuxtrack/                       # Libraries (RPATH points here!)
│       ├── liblinuxtrack.so.0
│       ├── libltr.so.0
│       └── ...
└── share/
```

### Why This Works:

1. **Binaries** in `/opt/linuxtrack/bin/` have **RPATH** pointing to `/opt/linuxtrack-0.99.19hid/lib/linuxtrack/`
2. **Config file** has `Prefix = "/opt/linuxtrack/bin"`
3. **Both directories exist** with necessary libraries
4. RPATH resolution finds `libltr.so.0` etc. in the `-0.99.19hid` directory
5. Config-based resolution (for Wine bridge) finds libraries via PREFIX path

---

## Why Fresh Builds Fail

When you rebuild linuxtrack with a new `--prefix`, several things can go wrong:

### Problem 1: Stale Build Artifacts

If you don't run `make distclean` before reconfiguring, old object files with old paths remain. The linker may use cached object files that still reference old directories.

**Solution:**
```bash
make distclean  # CRITICAL - removes all build artifacts
./configure --prefix=/opt/linuxtrack
make
```

### Problem 2: RPATH Mismatch

If binaries are compiled with one prefix but libraries are installed elsewhere:

```
Binary RPATH: /usr/local/lib/linuxtrack
Actual libs:  f/opt/linuxtrack/lib/linuxtrack

Result: Library not found!
```

### Problem 3: Config PREFIX Mismatch

If the config file points to a different location than where libraries exist:

```ini
# In ~/.config/linuxtrack/linuxtrack1.conf
Prefix = "/usr/local/bin"

# But libraries are in /opt/linuxtrack/lib/linuxtrack/
# Result: Wine bridge can't find liblinuxtrack.so.0
```

---

## Correct Build Procedure

### Step 1: Clean the Build Tree
```bash
cd /path/to/linuxtrack
make distclean
# If distclean fails (first build), just proceed
```

### Step 2: Configure
```bash
./configure
# Default prefix is now /opt/linuxtrack (as of 2025-12-25 fix)
# To use a different location: ./configure --prefix=/custom/path
```

**Note:** The `--with-wine-libs` option may be needed on some systems:
```bash
./configure --with-wine-libs="-L/usr/lib/wine -L/usr/lib/i386-linux-gnu/wine"
```

**DO NOT specify `--libdir` with `/linuxtrack` in it!** The build system automatically appends `/linuxtrack` to create `pkglibdir`.

### Step 3: Build
```bash
make
```

### Step 4: Install
```bash
sudo make install
```

### Step 5: Verify Installation
```bash
# Check RPATH matches install location
readelf -d /opt/linuxtrack/bin/ltr_server1 | grep RUNPATH
# Should show: /opt/linuxtrack/lib:/opt/linuxtrack/lib/linuxtrack

# Check libraries are found
ldd /opt/linuxtrack/bin/ltr_server1
# All libraries should resolve (no "not found")

# Check config file
grep -i prefix ~/.config/linuxtrack/linuxtrack1.conf
# Should show: Prefix = "/opt/linuxtrack/bin" (update via ltr_gui if needed)
```

---

## Troubleshooting

### Problem: "Library not found" at runtime

**Diagnosis:**
```bash
ldd /opt/linuxtrack/bin/ltr_server1 | grep "not found"
readelf -d /opt/linuxtrack/bin/ltr_server1 | grep RUNPATH
ls -la /opt/linuxtrack/lib/linuxtrack/
```

**Solutions:**

1. **If RPATH points to wrong location:** Rebuild with correct `--prefix`

2. **Quick fix with patchelf:**
   ```bash
   sudo patchelf --set-rpath '/opt/linuxtrack/lib:/opt/linuxtrack/lib/linuxtrack' \
       /opt/linuxtrack/bin/ltr_server1
   ```

3. **Workaround with LD_LIBRARY_PATH:**
   ```bash
   export LD_LIBRARY_PATH=/opt/linuxtrack/lib/linuxtrack:$LD_LIBRARY_PATH
   ```

### Problem: Tracking doesn't work (Wine games)

**Diagnosis:**
```bash
# Check what PREFIX the config file has
grep -i prefix ~/.config/linuxtrack/linuxtrack1.conf

# Check if libraries exist at that path
ls -la /opt/linuxtrack/lib/linuxtrack/liblinuxtrack.so.0
```

**Solution:** Run `ltr_gui`, make any change, save. This updates the PREFIX in config to match where `ltr_gui` is running from.

### Problem: Old binaries not getting updated

**Diagnosis:**
```bash
strings /opt/linuxtrack/bin/ltr_server1 | grep "/opt/"
# Check if paths match your intended installation
```

**Solution:** Full clean rebuild:
```bash
make distclean
./configure --prefix=/opt/linuxtrack
make clean  # Extra safety
make
sudo make install
```

---

## Path Variable Reference

| Variable | Derived From | Default Value | Used For |
|----------|--------------|---------------|----------|
| `prefix` | `--prefix` | `/usr/local` | Base installation directory |
| `exec_prefix` | `--exec-prefix` or `prefix` | `$(prefix)` | Executables base |
| `bindir` | `--bindir` | `$(exec_prefix)/bin` | Binary installation |
| `libdir` | `--libdir` | `$(exec_prefix)/lib` | Library base |
| `pkglibdir` | Automatic | `$(libdir)/linuxtrack` | Package libraries (RPATH target) |
| `datadir` | `--datadir` | `$(prefix)/share` | Data files |
| `pkgdatadir` | Automatic | `$(datadir)/linuxtrack` | Package data |

---

## Wine Bridge Specifics

The Wine bridge DLLs (`NPClient.dll`, `FreeTrackClient.dll`) use a different loading mechanism:

1. DLL is loaded by Wine process
2. DLL calls `linuxtrack_init()` 
3. `linuxtrack.c` reads PREFIX from config file
4. Constructs native library path and uses `dlopen()`
5. Native `liblinuxtrack.so.0` communicates with `ltr_server1` via shared memory

**Critical:** The Wine DLLs depend on the **config file PREFIX** being correct, not RPATH! The DLLs are PE format, not ELF - they don't have RPATH.

---

## Replicating the Dual-Directory Setup (If Needed)

If you need to exactly replicate the current working setup:

```bash
# Build with the 0.99.19hid prefix
make distclean
./configure --prefix=/opt/linuxtrack-0.99.19hid
make
sudo make install

# Optionally create the /opt/linuxtrack structure
sudo mkdir -p /opt/linuxtrack/bin
sudo ln -s /opt/linuxtrack-0.99.19hid/bin/* /opt/linuxtrack/bin/
# Or copy instead of symlink

# Update config to point to /opt/linuxtrack/bin
# (This happens automatically when you run ltr_gui from that location)
```

---

## Future Improvements

1. **Consider using `$ORIGIN` in RPATH:** This makes paths relative to the binary location:
   ```makefile
   LINUXLDFLAGS = '-Wl,-rpath,$$ORIGIN/../lib/linuxtrack'
   ```

2. **Document the PREFIX config behavior:** The auto-update of PREFIX when running ltr_gui should be documented in user-facing help.

3. **Add path validation:** `ltr_server1` could validate that libraries are findable at startup and provide clear error messages.

---

## Appendix: File Locations Summary

| File | Purpose |
|------|---------|
| `~/.config/linuxtrack/linuxtrack1.conf` | User config with PREFIX |
| `~/.config/linuxtrack/tir_firmware/` | Extracted TrackIR firmware |
| `/tmp/linuxtrack*.log` | Runtime log files |
| `/tmp/ltr_mmap2` | Shared memory for IPC |
| `src/Makefile.am` | Build system path definitions |
| `src/linuxtrack.c` | Library loading logic |
| `src/utils.c` | Path utility functions |
| `src/pathconfig.h` | Generated path configuration |
