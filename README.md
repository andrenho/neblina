# Neblina

<img src="https://upload.wikimedia.org/wikipedia/commons/3/35/Pico_da_Neblina_%28FAB%29.jpg" width="350">

A single binary application in C++ that provides a whole cloud. It's small and simple and it can be used on a small server (such as a Raspberry Pi).

## Philosophy

- Single binary
  - Binary size > simplicity > performance
  - Basic services part of the binary
  - New services can be added as .so plugins
  - Runs on any POSIX OS (Windows, Linux (PC or Pi), Mac, BSD, Android)
- Avoid external libraries
  - Rebuild the wheel
  - Use external libraries for security stuff (static library in this case (?))
- Easy installation
  - Just drop the binary and run it, or run as a service
 
# Architecture

- Binary is initialized without options
  - Run watchdog, which starts other services
  - Network services run as a fork of the original binary
  - One network service = one port
- Special ports
  - 443 -> offload security and redirect to 80
  - 80 -> special service Web Listener
  - Other packages -> redirect to their Internet handlers (.so or native)
- Web listener
  - Pure web page (HTML) - just read files
  - Web application (REST)
    - each Web application can be it's own .so
- Web application
  - REST only
  - Language to generate REST classes (from OpenAPI)
  - Language to generate DAOs/Entities (from custom language ?) - connect to sqlite
  - Authentication (support to OAuth2)
- Other services
  - Scheduled services - run at specific times

# Services

- Configuration (web application)
- Dynamic DNS (scheduled application)
- DNS (including dynamic)
- FTP (network service)
- HTTP (network service - special)
- HTTPS (network service)
- SFTP (network service)

# Rules for new modules

- Modules will use a section of the nebula.conf file for their own configuration
- If a section is not present, the module should create it at first use