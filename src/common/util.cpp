
#include <stdio.h>
#include <util.h>
namespace common {
    evutil_socket_t create_listen_socket(int addr, int port) {
        evutil_socket_t listener;
        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.s_addr = addr;
        listener = socket(AF_INET, SOCK_STREAM, 0 );
        evutil_make_socket_nonblocking(listener);
        
        int one = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
        
        if (-1 == bind(listener, (struct sockaddr*)&sin, sizeof(sin)) ) {
            perror("bind");
            return -1;
        }
        
        if (listen(listener, 16)<0) {
            perror("listen");
            return -1;
        }
        return listener;
    }
    
    
}