typedef enum{
	SHUTDOWN,
} commands;

typedef struct{
	commands command;
	void* (*handler)(void*);
} command_t;