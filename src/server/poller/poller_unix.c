#include "poller.h"

#include <sys/epoll.h>

#include "common.h"

static int epoll_fd = -1;
static int fs_socket = -1;

void poller_init(int fd_listener)
{
    epoll_fd = epoll_create1(0); // 0 for default flags
    if (epoll_fd < -1)
        FATAL("Could not initialize epoll: %s", strerror(errno));

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd_listener;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_listener, &event) < 0)
        FATAL("Could not initialize socket fd in epoll: %s", strerror(errno));

    fs_socket = fd_listener;
}

bool poller_add_connection(int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP | EPOLLERR;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
        THROW("Could not initialize socket fd in epoll: %s", strerror(errno));

    return true;
}

bool poller_remove_connection(SOCKET fd)
{
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0)
        THROW("Error deleting socket fd in epoll: %s", strerror(errno));
    return true;
}

size_t poller_wait(PollerEvent* out_evt, size_t evt_sz)
{
    struct epoll_event events[evt_sz];
    size_t n_events = epoll_wait(epoll_fd, events, (int) evt_sz, 100);

    for (size_t i = 0; i < n_events; ++i) {
        if (events[i].data.fd == fs_socket) {
            out_evt[i] = (PollerEvent) { .type = PT_NEW_CONNECTION, .fd = fs_socket };
        } else {
            if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                out_evt[i] = (PollerEvent) { .type = PT_DISCONNECTED, .fd = events[i].data.fd };
            } else if (events[i].events & (EPOLLIN | EPOLLET)) {
                out_evt[i] = (PollerEvent) { .type = PT_NEW_DATA, .fd = events[i].data.fd };
            }
        }
    }

    return n_events;
}