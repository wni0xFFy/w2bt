#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include "include/socket.h"
#include <stdlib.h>

sock_t* create_socket(int port){
	sock_t* s = malloc(sizeof(sock_t));
	if(s == NULL) return s;

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		free(s);
		return NULL;
	};

	struct sockaddr_in settings;
	settings.sin_family = AF_INET;
	settings.sin_addr.s_addr = INADDR_ANY;
	settings.sin_port = htons(port);

	int err = bind(fd, (struct sockaddr*)&settings, sizeof(settings));
    if(err == -1) {
    	free(s);
    	close(fd);
        return NULL;
    }

    err = listen(fd, 5);
    if(err == -1){
    	free(s);
    	close(fd);
    	return NULL;
    }
    int flags = fcntl(fd, F_GETFL, 0);
	flags &= ~O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);

    s->fd = fd;
    s->settings = settings;

    return s;
}

void destroy_socket(sock_t* sock){
	close(sock->fd);
	free(sock);
}

sock_t* accept_socket(sock_t* self){
	if(self == NULL) return NULL;

	sock_t* client = malloc(sizeof(sock_t));

	client->settings_size = sizeof(struct sockaddr_in);
	client->fd = accept(self->fd, (struct sockaddr*)&client->settings, (socklen_t*)&client->settings_size);

	if(client->fd == -1){
		free(client);
		return NULL;
	}
	return client;
}

int receive_socket(sock_t* client, void* buffer, uint16_t size, uint16_t offset){
	if(client == NULL || buffer == NULL) return -1;
	int readed = read(client->fd, buffer + offset, size - offset);

	if(readed == size) return size;
	else if(readed < 0) return -1;
	else if(readed == 0) return -2;
	else return readed;
}

int send_socket(sock_t* client, void* buffer, uint16_t size, uint16_t offset){
	if(client == NULL || buffer == NULL) return -1;
	int sended = send(client->fd, buffer + offset, size - offset, 0);

	if(sended == size) return 0;
	if(sended < 1) return -1;
	else return sended;	
}

void nonblocking_socket(sock_t* sc) {
    int flags = fcntl(sc->fd, F_GETFL, 0);
    fcntl(sc->fd, F_SETFL, flags | O_NONBLOCK);
}