# SCS Linuxtrack Plugin - README

Native head tracking plugin for **Euro Truck Simulator 2** and **American Truck Simulator** on Linux.

## Installation

1. **Build the plugin** (if not already done):

   ```bash
   cd /path/to/linuxtrack
   cmake -B build -S .
   cmake --build build --target scs_linuxtrack
   ```

2. **Copy to game plugins folder**:

   ```bash
   # For ETS2:
   mkdir -p ~/.local/share/Steam/steamapps/common/Euro\ Truck\ Simulator\ 2/bin/linux_x64/plugins/
   cp build/src/scs_plugin/scs_linuxtrack.so ~/.local/share/Steam/steamapps/common/Euro\ Truck\ Simulator\ 2/bin/linux_x64/plugins/
   
   # For ATS:
   mkdir -p ~/.local/share/Steam/steamapps/common/American\ Truck\ Simulator/bin/linux_x64/plugins/
   cp build/src/scs_plugin/scs_linuxtrack.so ~/.local/share/Steam/steamapps/common/American\ Truck\ Simulator/bin/linux_x64/plugins/
   ```

3. **Start Linuxtrack** before launching the game:

   ```bash
   ltr_gui  # Or ensure ltr_server1 is running
   ```

4. **Launch the game**. The plugin will connect to Linuxtrack automatically.

## Requirements

- `ltr_server1` must be running (started by `ltr_gui`)
- TrackIR hardware or webcam must be configured in `ltr_gui`
- `liblinuxtrack.so` must be in library path (installed to `/opt/linuxtrack/lib/linuxtrack/`)

## Current Limitations

> **Important**: The SCS Telemetry SDK is OUTPUT-only. It provides read access to game telemetry but does not currently support INPUT injection.

This means:

- The plugin CAN read game state (truck position, speed, etc.)
- The plugin CAN read head pose from Linuxtrack
- The plugin CANNOT directly control the in-game camera (yet)

For actual camera control, we would need:

1. SCS to add an official Input API (feature request)
2. TrackIR shared memory emulation (Linux implementation of NaturalPoint memory interface)
3. Or continue using `ltr_pipe` joystick emulation (current workaround)

## Debugging

Plugin logs to `stderr`. Run the game from terminal to see output:

```bash
~/.local/share/Steam/steamapps/common/Euro\ Truck\ Simulator\ 2/bin/linux_x64/eurotrucks2 2>&1 | grep scs_linuxtrack
```
