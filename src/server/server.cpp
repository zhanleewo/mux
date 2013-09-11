//
//  MuxServer.cpp
//  Mux
//
//  Created by admin on 13-8-26.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#include <server.h>
#include <util.h>
#include <unistd.h>

#include <sys/socket.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_compat.h>

#define PORT_SERVER 8888
#define MAX_LINE 16384   //16kb



char rot13_char(char c)
{
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}
MuxServer MuxServer::_instance;
void MuxServer::readcb(struct bufferevent *bev, void *ctx)//读缓冲满了时的处理函数
{
    struct evbuffer *input, *output;
    char *line;
    size_t n;
    int i;
    
    input = bufferevent_get_input(bev);//一个socket,既可以用来输入也可以用来输出，我们不用send和receive，只对缓冲区进行操作
    output = bufferevent_get_output(bev);//它们分别返回输入和输出缓冲区
    
    //    char buf[1204];
    int fd = bufferevent_getfd(bev);
    //    evbuffer_read(input, fd, 1024);
    //    evbuffer_remove(input, buf, 1024);
    
    while ((line = evbuffer_readln(input, &n, EVBUFFER_EOL_LF))) {//按行从缓冲区读取，行终结符为\n
        printf("===> %s\n", line);
        if(strncmp(line, "MuxCMDInit", 10) == 0) {
            printf("MuxCMDInit\n");
            for (std::list<client_entry>::iterator iter = MuxServer::_instance.entrys.begin(); iter != MuxServer::_instance.entrys.end(); iter++) {
                if((*iter).clientfd == fd) {
                    uuid_string_t struuid;
                    uuid_unparse_upper((*iter).uuid, struuid);
                    evbuffer_add(output, struuid, strlen(struuid));
                    evbuffer_add(output, "\n", 1);
                }
            }
            evbuffer_add(output, "OK\n", 21);
        } else if(strncmp(line, "MuxCMDData", 10) == 0) {
            printf("MuxCMDData\n");
            evbuffer_add(output, "recv 1024 bytes\n", 16);
        }
        
        
        //        for (i = 0; i < n; ++i)
        //            line[i] = rot13_char(line[i]);
        //        evbuffer_add(output, line, n); //将bit输出到输出缓冲区的终端
        //        evbuffer_add(output, "\n", 1);
        //        free(line);
    }
    
    if (evbuffer_get_length(input) >= MAX_LINE) {  //读取输入缓冲区中的字节数
        //如果太长，以至于前面无法把它当成是一行
        /* Too long; just process what there is and go on so that the buffer
         * doesn't grow infinitely long. */
        char buf[1024];
        while (evbuffer_get_length(input)) {
            int n = evbuffer_remove(input, buf, sizeof(buf)); //从缓冲区input复制buf长度的数据到buf
            for (i = 0; i < n; ++i)
                buf[i] = rot13_char(buf[i]);
            evbuffer_add(output, buf, n);
        }
        evbuffer_add(output, "\n", 1);
    }
}

void MuxServer::errorcb(struct bufferevent *bev, short error, void *ctx)//错误处理回调函数
{
    if (error & BEV_EVENT_EOF) {
        /* connection has been closed, do any clean up here */
        /* ... */
    } else if (error & BEV_EVENT_ERROR) {
        /* check errno to see what error occurred */
        /* ... */
    } else if (error & BEV_EVENT_TIMEOUT) {
        /* must be a timeout event handle, handle it */
        /* ... */
    }
    bufferevent_free(bev);
}

int MuxServer::parse_commond_line(int argc, const char **argv) {
    return 0;
}

MuxServer& MuxServer::instance(int argc, const char **argv) {
    if(MuxServer::parse_commond_line(argc, argv) != 0) {
        throw "parse commond args error.";
    }
    return _instance;
}

void MuxServer::do_accept(int listener, short event, void *arg) {
    MuxServer *_this = (MuxServer *) arg;
    struct event_base *base = (struct event_base *) _this->_base;
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        perror("accept");
    } else if (fd > FD_SETSIZE) {
        close(fd);
    } else {
        
        client_entry entry = {
            .uuid = {0},
            .destfd = -1,
            .clientfd = fd,
        };
        uuid_generate(entry.uuid);
        uuid_string_t struuid;
        uuid_unparse_upper(entry.uuid, struuid);
        std::cout << "UUID : " << struuid << std::endl;
        _this->entrys.push_back(entry);
        struct bufferevent *bev; //进行读写工作
        evutil_make_socket_nonblocking(fd);
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, MuxServer::readcb, NULL, MuxServer::errorcb, NULL);
        bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
        
    }
}

void* MuxServer::thread_fun(void *args) {
    MuxServer *_this = (MuxServer *) args;
    evutil_socket_t listener;
    struct event *listener_event;
    struct event_base *base = (struct event_base *) _this->_base;
    
    listener = common::create_listen_socket(INADDR_ANY, 8888);
    evutil_make_socket_nonblocking(listener);
    evutil_make_listen_socket_reuseable(listener);
    
    listener_event = event_new(base, listener, EV_READ|EV_PERSIST, MuxServer::do_accept, (void*)_this);
    event_add(listener_event, NULL);
    event_base_dispatch(base);
    return NULL;
}


void MuxServer::run() {
    // 地址
    if(_base == NULL) {
        _base = event_base_new();
    }
    pthread_create(&_loop_thread, NULL, MuxServer::thread_fun, this);
    pthread_join(_loop_thread, NULL);
}

void MuxServer::stop() {
    struct timeval tv = {.tv_sec=1, .tv_usec=0};
    event_base_loopexit(_base, &tv);
    pthread_join(_loop_thread, NULL);
}
