### v0.1 - JSON -> C++

- [x] Application to generate C++ classes to parse JSON objects

### v0.2 - Orchestrator

- [x] Create base application
- [x] Support for embedded file
- [x] Config file and data dir infrastructure
- [x] Orchestrator
  - [x] Read config file
  - [x] Keep track of running services
- [x] Logging
- [x] Parrot
- [x] Eliminate templates from connection
- [x] Replace tcp_server_text.* by tcp_line_connection.*
- [x] Listen local/world

### v0.3 - HTTP service

- [x] Implement HTTP service (consider usability from other connection types)
- [ ] Create static HTTP service
- [ ] Create basic support to WebApplication
- [ ] Basic authentication

Extra:
- [x] Move config file to "/config"

### v0.4 - Interservice communication

- [ ] Interservice communication
  - [ ] Listen on named pipes
  - [ ] Small HTTP service for processing messages
- [ ] Implement TCP service
- [ ] Implement scheduled service
- [ ] Load .so service
- [ ] Compile on Windows

### v0.5 - Web Application

- [ ] `json2cpp` -> write to json
- [ ] REST interface
- [ ] Database access
- [ ] Tool to generate entities and DAOs
- [ ] Create sample "guestbook" service as .so

### v0.6 - HTTPS

- [ ] HTTPS support

### v0.7 - Authentication

- [ ] OAuth2 support
- [ ] Web tool to manage users

### v0.8 - Other services

- [ ] FTP
- [ ] SFTP
- [ ] XMPP
- [ ] NNTP
- [ ] Gopher

### v0.9 - First execution

- [ ] Ask for password and configuration when using for the first time

### v0.10 - Advanced web applications

- [ ] Password manager
- [ ] Note taking
- [ ] Notepad

### ???

- [ ] Pass parameters as custom parameters
- [ ] Combine multiple services in one (?)
- [ ] Create named pipe / socket / shared memory connection

