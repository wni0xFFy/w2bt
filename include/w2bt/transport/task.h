#include <w2bt/core/socket.h>

typedef enum {
	READ, 
	WRITE,
} MODE;

typedef struct{
	sock_t* sc;
	uint8_t* buffer;
	MODE function;
} task_t;

