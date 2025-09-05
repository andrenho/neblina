# pita
A single binary service in C++ that replaces most of online popular services.

- Single binary
  - Basic services are part of the binary (?)
  - External services as .so plugins
  - Runs on any POSIX OS (Windows, Linux, Mac, BSD, Android)
- Avoid external libraries
  -  Use external libraries for security stuff (static library in this case (?))

# Layers

- Deep backend
  - Protobuf + automatic REST (?)
- BFF
- Frontend

# Services

- Installation
- DNS (including dynamic)
- File service (drive)
