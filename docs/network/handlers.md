network/handlers.c and include/network/handlers.h

Handlers for states of task_t instances.

# FUNCTIONS

1. int accept_handler(sock_t* serv, fd_poll_t* epfd) - accept client from epoll.
2. int read_handler(task_t* t) - read data from task_t's client.

