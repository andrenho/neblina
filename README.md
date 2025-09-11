# Neblina

<img src="https://upload.wikimedia.org/wikipedia/commons/3/35/Pico_da_Neblina_%28FAB%29.jpg" width="350">

A single binary application in C++ that provides a whole cloud. It's small and simple and it can be used on a small server (such as a Raspberry Pi).

# Philosophy

- Single binary
  - Binary size > simplicity > performance
  - Native services part of the binary
  - New services can be provided as .so plugins
  - Runs on any POSIX OS (Windows, Linux (PC or Pi), Mac, BSD, Android)
- Avoid external libraries
  - No dependencies except for the ones already provided by the OS
  - Reinvent the wheel
  - Use external libraries for security stuff (static library in this case (?))
- Easy installation
  - Just drop the binary and run it, or run as a service
 
# Architecture

- Neblina architecture is composed of **services**
  - When neblina is run, an **Orchestrator** service is started
    - The Orchestrator starts other services by _forking_ the main application
    - It keeps track of services that died or hanged, and restarts them
  - **HTTP** is a special service
    - Listen to HTTP connections, and redirects the connection to other services based on path/host
- Services can be of the following types:
  - **Scheduled**: executed at some frequency or at specific times
  - **TCPService**: listen to a TCP port. (TCPServiceText is the same but works on whole lines instead of bytes)
  - **WebApplication** is a special type of service. It's meant for creating REST applications. The following
         functionalities are provided to help on this process:
    - HTTP/REST interface
    - Authentication (simple and OAuth)
    - Database access (SQLite3)
    - Tools to generate C++ classes to represent JSON objects (`json2cpp`), DAOs and database entities
- Every service opens a named pipe with its name
  - This can be used for interservice communication
  - Communication happens by exchanging JSON messages via the named pipe
  - TODO - define protocol
- Config files and data directory
  - The data directory determines where data lives. If a different directory is chosen, it's the same as a completely
    different execution
  - Data directory contains the following subdirectories:
    - config: for storing config files (JSON)
    - secret: for storing user information (passwords, etc)
    - data: each service has its own subdirectory
  - Each service should have one JSON config file with the name of the service (`service_name.json`)
    - A config file with sane defaults should be created on the config directory
    - The classes for reading the config can be auto-generated using the `json2cpp` tool.

# Services

- **Orchestrator**: manages the other services
- **HTTP**: receive HTTP requests, and redirects them to other services via named pipes
- **HTTPS**: receive HTTPS, offloads SSL handshake and redirect connection to HTTP
- **FTP**: ftp server
- **SFTP**: offloads SSL and redirects to FTP
- **XMPP**: chat server
- **NNTP**: news server
- **Gopher**

Basic web applications:

- **Configurator**: perform the first steps, and allow users to change config files
- **Oauth2**: provides authentication

Advanced web applications

- **Note taking**
- **Password manager**
- **Notepad**