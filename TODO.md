network.c/create_socket_poll()
	write checks in malloc()

network module must contain:
	1. creating epoll [+]
	2. deleting and free() epoll. [+]
	3. adding tasks [+]
	5. handle tasks
	6. return error values