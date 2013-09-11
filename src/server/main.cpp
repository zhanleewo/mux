#include <iostream>
#include <server.h>

int main(int argc, char **argv) {
    MuxServer &server = MuxServer::instance();
    server_config cfg;
    server.run(cfg);
	return 0;
}
