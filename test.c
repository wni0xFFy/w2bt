#define MAX_EVENTS 5
#define READ_SIZE 10
#include <stdio.h>     // for fprintf()
#include <unistd.h>    // for close(), read()
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <string.h>    // for strncmp
//РАЗОБРАТЬ ЭТОТ КОД И ВНЕДРИТЬ ЕГО В network.c ТАК ЧТО БЫ ПРИ СОЗДАНИИ СЕРВЕРА
// ИВЕНТ fd СЕРВЕРА БЫЛ ВСЕГДА В ACCEPT И НАПИСАТЬ ОБЕРТКУ ЧТО БЫ МОЖНО БЫЛО 
//КЛИЕНТСКИЙ СОКЕТ ТУПО В ФУНКЦИЮ ЗАКИНУТЬ И ОНА САМА ЕГО В epoll ДОБАВЛЯЛА

int main()
{
	int event_count;
	size_t bytes_read;

	char read_buffer[READ_SIZE + 1];

	struct epoll_event event, events[MAX_EVENTS], yaevent;
	int epoll_fd = epoll_create1(0);

	if (epoll_fd == -1) {
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return 1;
	}

	event.events = EPOLLIN;
	event.data.ptr = sc;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
	{
		fprintf(stderr, "Failed to add file descriptor to epoll\n");
		close(epoll_fd);
		return 1;
	}

	while (1) {
		event_count = epoll_wait(epoll_fd, events, 10, -1);

		for (int i = 0; i < event_count; i++) {
			bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
			read_buffer[bytes_read] = '\0';
		}
	}

	if (close(epoll_fd)) {
		fprintf(stderr, "Failed to close epoll file descriptor\n");
		return 1;
	}

	return 0;
}