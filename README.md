# pita
A single binary service in C++ that replaces most of online popular services.

- Single binary
  - Basic services are part of the binary (?)
  - External services as .so plugins
  - Runs on any POSIX OS (Windows, Linux (PC or Pi), Mac, BSD, Android)
- Focus on low-spec platform
  - Focus on low memory usage, low CPU usage
- Avoid external libraries
  -  Use external libraries for security stuff (static library in this case (?))
- Easy installation
  - Just drop the binary and run it, or run as a service

# Layers

- Deep backend
  - Protobuf + automatic REST (?)
- BFF
- Frontend

# Services

- Installation
- DNS (including dynamic)
- File service (drive)
