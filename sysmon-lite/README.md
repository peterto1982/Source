
# sysmon-lite

Small Linux C++20 monitoring demo.

## Features
- CPU usage (/proc/stat)
- Memory usage (/proc/meminfo)
- Disk usage (std::filesystem)
- Network RX/TX (/proc/net/dev)
- Four worker threads
- Shared data protected by std::mutex

## Build

```bash
mkdir build
cd build
cmake ..
make -j
./sysmon
```
