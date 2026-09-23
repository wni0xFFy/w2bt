#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>

int accept_handler(sock_t* serv, fd_poll_t* epfd);
int read_handler(task_t* t, uint8_t* buffer, int size);
int close_handler(fd_poll_t* epl, task_t* t);