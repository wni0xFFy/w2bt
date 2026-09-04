#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include "include/socket.h"
sock_t create_socket(int port){
	sock_t s;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		return s;
	}

	struct sockaddr_in settings;
	settings.sin_family = AF_INET;
	settings.sin_addr.s_addr = INADDR_ANY;
	settings.sin_port = htons(port);

	int err = bind(fd, (struct sockaddr*)&settings, sizeof(settings));
    if(err == -1) {
        return s;
    }
    listen(fd, 5);

    s.fd = fd;
    s.settings = settings;

    return s;
}

sock_t accept_socket(sock_t* master){
	sock_t slave;
	slave.fd = accept(master->fd, (struct sockaddr*)&slave.settings, (socklen_t*)&slave.settings_size);
	return slave;
}