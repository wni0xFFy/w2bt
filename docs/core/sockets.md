core/socket.c and core/include/socket.h

This module is abstraction over sockets. 

functions
1. create_socket() - creates sock_t structure 
2. destroy_socket() - closes file descriptor and free memory
3. accept_socket() - accept client socket
4. receive_socket() - receive from client
5. send_socket() - send to client
6. nonblocking_socket() - set NONBLOCKING option to socket. 

# ---------STRUCTURES--------
	```C
		typedef struct{
			int fd; 
			struct sockaddr_in settings;
			int settings_size;
		} sock_t;
	```
	sock_t structure is a main structure in this module every function need it.

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
		sock_t* create_socket(int port, int backlog);
	```
	Create blocking socket.
	Request socket file descriptor from OS, bind it with port, change mode to "listen". 

	@param 
		1. port(int) -> port you need to create socket with
		2. backlog(int) -> backlog for listen() function

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
		client sock_t* - it've been malloced and returned. instance of sock_t contains client socket fd.
		null - some error

	@note
		RETURN VALUE MUST REMOVE FROM RAM AND CLOSE SOCKET VIA destroy_socket().

	```C
		char* receive_socket(sock_t* client, void* buffer, uint16_t size, uint16_t offset);
	```

	Receives data from client(blocks thread and wait client without nonblocking).

	@param 
		1. client(sock_t*) - pointer to sock_t structure instance(gets via accept_socket())
		2. buffer(void*) - buffer that must has lengt >= size.
		2. size(uint_16) - size of buffer
		3. offset(uint16_t) - offset means readed data. you should transfer that to function(if last receive_socket() has returned NOT 0).

	@return
		-1 - read returns -1.
		-2 - client closed connection.
		something else - readed data lenght.

	```C
		int send_socket(sock_t* client, void* buffer, uint16_t size, uint16_t offset);
	```

	Send data to client

	@param 
		1. client(sock_t*) - client socket struct(gets via accept_socket())
		2. buffer(void*) - data which will be sended
		3. data - size of param.2
		4. offset - sended size(if last send didn't return 0)

	@return
		-1 - error
		0 - means ALL data sended
		else - return sended lenght

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
	uint16_t buf[256];
	receive_socket(c, buf, 256, 0); //reads 256 bytes data from client
	send_socket(c, buf, 256, 0); //send data to client

	destroy_socket(c);//
	destroy_socket(s);// CLEARING
	free(data); 	  //
```