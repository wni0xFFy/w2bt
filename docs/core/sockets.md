core/socket.c and core/include/socket.h

This module is abstraction over sockets. 
It can use to :
	1. creating sockets
	2. write to sockets
	3. read from sockets

# ---------STRUCTURES--------
	```C
		typedef struct{
			int fd; 
			struct sockaddr_in settings;
			int settings_size;
		} sock_t;
	```
	sock_t structure is a main structure in this module. every function need it.

	@fields
		1. fd(int) - contains socket file destriptor
		2. settings(struct sockaddr_in) - contains data like domain, port, etc.
		3. settings_size(int) - size of param.2

	@size
		that struct is 24 bytes long.

# ---------FUNCTIONS---------

	```C
		void destroy_socket(sock_t* sock);
	```
	Close socket and call free() for structure.

	@param
		1. sock(sock_t*) -> pointer to sock_t structure instance you need to destroy

	```C
		sock_t* create_socket(int port);
	```
	Create blocking socket.
	Request socket file descriptor from OS, bind it with port, change mode to "listen". 

	@param 
		1. port(int) -> port you need to create socket with

	@return
		pointer to socket structure instance.
		null - some error

	@note
		RETURN VALUE MUST REMOVE FROM RAM AND CLOSE SOCKET VIA destroy_socket().

	```C
		sock_t* accept_socket(sock_t* self);
	```

	Accept client connection(blocks thread and wait connection)

	@param 
		1. self(sock_t*) - server socket structure pointer(gets via create_socket)

	@return
		client sock_t* - it've been malloced and returned. instance of sock_t contains client socket fd
		null - some error

	@note
		RETURN VALUE MUST REMOVE FROM RAM AND CLOSE SOCKET VIA destroy_socket().

	```C
		char* receive_socket(sock_t* client, int data_size);
	```

	Receives data from client(blocks thread and wait data). It rerequest data if data_size(param 1) != readed_data_size

	@param 
		1. client(sock_t*) - pointer to sock_t structure instance(gets via accept_socket())
		2. data_size(int) - expected size of received data.

	@return
		char* buffer - malloced buffer with readed data.
		null - some error

	@note 
		returnal buffer must will be free()

	```C
		int send_socket(sock_t* client, char* buffer, int data_size);
	```

	Send data to client

	@param 
		1. client(sock_t*) - client socket struct(gets via accept_socket())
		2. buffer(char*) - data that will be sended
		3. data_size - size of param.2

	@return
		-1 - error
		0 - ok

	```C
		void nonblocking_socket(sock_t* sc)
	```

	Make socket nonblocking

	@param 
		1. sc(sock_t*) - pointer to sock_t instance which you need make nonblocking

# -----EXAMPLES------

How to use it?
```C
	sock_t* s = create_socket(8080); //creates socket and listen it
	sock_t* c = accept_socket(s); // accept connection

	char* data = receive_socket(c, 256); //reads 256 bytes data from client
	send_socket(c, data, 256); //send data to client

	destroy_socket(c);//
	destroy_socket(s);// CLEARING
	free(data); 	  //
```