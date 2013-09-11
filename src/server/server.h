//
//  MuxServer.h
//  Mux
//
//  Created by admin on 13-8-26.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#ifndef __Mux__MuxServer__
#define __Mux__MuxServer__

#include <iostream>
#include <list>
#include <uuid/uuid.h>

struct event_base;


class MuxServer {
public:
    static MuxServer& instance(int argc, const char **argv);
    void run();
    void stop();
protected:
    static int parse_commond_line(int argc, const char **argv);
    static void do_accept(int listener, short event, void *arg);
    static void *thread_fun(void *args);
    static void readcb(struct bufferevent *bev, void *ctx);
    static void errorcb(struct bufferevent *bev, short error, void *ctx);
private:
    typedef struct _client_entry {
        uuid_t uuid;
        int destfd;
        int clientfd;
    } client_entry;
    
    typedef struct _server_config {
        int listenPort;
    } server_config;
    struct event_base *_base;
    pthread_t _loop_thread;
    static MuxServer _instance;
    std::list<client_entry> entrys;
    server_config _cfg;
};
#endif /* defined(__Mux__MuxServer__) */
