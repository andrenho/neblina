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
- [x] Create basic support to WebApplication
  - [x] Health request handler
  - [x] Keep working on matching
- [x] Create static HTTP service
  - [x] HTTP not closing connection (?)
  - [x] index.html
- [x] Static website using files
- [x] Static website using compressed files
  - [x] New compression scheme - use gzip
  - [x] Allow adding full directories in the compression
  - [x] Create initialization procedure (It works, config files, etc)
- [x] Extra HTTP stuff
  - [x] Require Host request (otherwise 400)
  - [x] Automatic error according to request type
  - [x] Persistent connections (check Connection header)
  - [x] Transfer encoding: gzip, deflate
  - [x] Redirects
  - [x] Conditional requests: If-None-Match, ETag

Extra:
- [x] Move config file to "/config"
- [x] Rename http_router to http

### v0.3a - Code improvement

- [x] Tests
  - [x] Start application
  - [x] Simple request (health check)
  - [x] Redirect
  - [x] Website
  - [x] ETag
  - [x] Orchestrator
- [x] "active" in orchestrator
- [x] Code review

### v0.4 - HTTPS

- [x] HTTPS support
- [x] Create proxy
  - [x] Create TCP client
  - [x] Remove current path
  - [x] Capture exception when calling TCP client
  - [x] When removing part of path, handle redirection 304
  - [x] Add originator IP
- [x] HTTPS proxy
- [x] Bug: connection failing when connecting on 8080
  - Connections not being closed
- [x] CONNECT
- [x] forward/ignore hop-by-hop headers if proxying
- [x] Serve from 443
- [x] HTTP-to-HTTPS
  - [x] Redirection 301

### v0.4a - reorganize config

- [ ] valgrind (finalize connection)
- [ ] Single config file
- [ ] Windows version (basic), Mac version

### v0.5 - Other services

- [ ] FTP
- [ ] SFTP
- [ ] XMPP
- [ ] NNTP
- [ ] Gopher (as a .so)

### v0.6 - Initial release

- [ ] Webpage
- [ ] Code review
- [ ] Add version number
- [ ] Make install
- [ ] Prepare for release
- [ ] Run tests on github

### v0.7 - Web Application

- [ ] `json2cpp`
  - [ ] Improve
  - [ ] Add tests
  - [ ] Add unordered_map
- [ ] Parse QueryParameters on HttpConnection::find_request_handler
- [ ] REST interface
- [ ] Database access
- [ ] Tool to generate entities and DAOs
- [ ] Create sample "guestbook" service as .so
- [ ] CORS
  - [ ] OPTIONS
- [ ] If a client sends Expect: 100-continue, the server must either respond 100 Continue or a final status code.
- [ ] Proxy

### v0.8 - Authentication

- [ ] Basic auth
- [ ] OAuth2 support
- [ ] Web tool to manage users

### v0.9 - Config tool

- [ ] Web tool for config
- [ ] Ask for password and configuration when using for the first time
- [ ] Initial setup

### v0.10 - Advanced web applications

- [ ] Markdown webpage
- [ ] Password manager
- [ ] Note taking
- [ ] Notepad
- [ ] Blog
- [ ] Social network
- [ ] Photo/video app

### v0.11 - Advanced web server
- [ ] Transfer-Encoding (chunks)
- [ ] Streaming
- [ ] Caching: If-Modified-Since, Last-Modified
- [ ] Support everything in the RFCs
- [ ] Control the time that the connection is open, close it after a while
- [ ] Dock app on Windows, Linux

### ???

- [ ] Pass parameters as custom parameters
- [ ] Combine multiple services in one (?)
- [ ] Create named pipe / socket / shared memory connection

