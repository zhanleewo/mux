//
//  util.h
//  mux
//
//  Created by admin on 13-9-10.
//
//

#ifndef mux_util_h
#define mux_util_h

#include <event2/event.h>
namespace common {
    evutil_socket_t create_listen_socket(int addr, int port);
}
#endif