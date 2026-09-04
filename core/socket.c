#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include "include/socket.h"
#include <stdlib.h>

sock_t* create_socket(int port){
	sock_t* s = malloc(sizeof(sock_t));
	if(s == NULL) return s;

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1) return NULL;

	struct sockaddr_in settings;
	settings.sin_family = AF_INET;
	settings.sin_addr.s_addr = INADDR_ANY;
	settings.sin_port = htons(port);

	int err = bind(fd, (struct sockaddr*)&settings, sizeof(settings));
    if(err == -1) {
    	close(fd);
        return NULL;
    }

    err = listen(fd, 5);
    if(err == -1){
    	close(fd);
    	return NULL;
    }
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

	client->settings_size = sizeof(sockaddr_in);
	client->fd = accept(self->fd, (struct sockaddr*)&client->settings, (socklen_t*)&client->settings_size);

	if(client->fd == -1){
		free(client);
		return NULL;
	}
	return client;
}

char* receive_socket(sock_t* client, int data_size){
	if(client == NULL) return NULL;

	char* buffer = malloc(data_size);
	if(buffer == NULL){
		return NULL;
	}
	int size = data_size;
	while(data_size > 0){
		int readed = read(client->fd, buffer, data_size);
		if(readed < 1){
			free(buffer);
			return NULL;
		}
		data_size -= readed;
		buffer += readed;
	}
	return buffer - size; 
}

int send_socket(sock_t* client, char* buffer, int data_size){
	if(client == NULL || buffer == NULL) return -1;
	send(client->fd, buffer, data_size, 0);	
	return 0;
}