#include <iostream>
#include <event2/event.h>
#include <util.h>

static int i = 0;
void timeout_event_callback(evutil_socket_t s, short evs, void * args) {
    struct event_base *base = (struct event_base *) args;
    if(i++ == 10) {
        printf("End at %d time out\n", i);
        event_base_loopexit(base, NULL);
        return;
    }
    printf("%d time out\n", i);
    
    struct event *ev = event_new(base, 0, EV_TIMEOUT, timeout_event_callback, base);
    struct timeval tv = {.tv_sec=1, .tv_usec=0};
    event_add(ev, &tv);
}

int main(int argc, char **argv) {

    evutil_socket_t server_socket = common::create_listen_socket(INADDR_ANY, 8888);
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(server_socket, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        perror("accept");
    } else if (fd > FD_SETSIZE) {
        close(fd);
    } else {
    }
    evutil_make_listen_socket_reuseable(server_socket);
    evutil_make_socket_nonblocking(server_socket);
    
	struct event_base *base = event_base_new();
    struct event *ev = event_new(base, 0, EV_TIMEOUT, timeout_event_callback, base);
    struct timeval tv = {.tv_sec=1, .tv_usec=0};
    event_add(ev, &tv);
    event_base_dispatch(base);
	return 0;
}
