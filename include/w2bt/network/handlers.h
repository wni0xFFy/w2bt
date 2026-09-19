#include <w2bt/network/network.h>
#include <w2bt/core/socket.h>

int accept_handler(sock_t* serv, fd_poll_t* epfd);
int read_handler(task_t* t);