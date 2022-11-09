# Webserv Functions

## H2N & N2H
Network byte order defines the bit-order of network addresses as they pass through the network. 
</br> htons() (host to network short 16 bytes) & htonl() (host to network long 32 bytes) 
are host calls to put port numbers into network byte orders. </br>
</br>
```asm
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
```

Using network byte ordering for data exchanged between hosts allows hosts using different architectures 
to exchange address information without confusion because of byte ordering.
ntohl() && ntohs() functions put the network byte ordering into the host byte ordering. 
```asm
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
```

## Multiplexing: Poll & Kqueue
Multiplexing means that multiple signals are combined to one signal. For That select, poll or epoll 
can be used. </br>

### Select()
A call to select() will block until the given file descriptors are ready to perform I/O, 
or until an optionally specified timeout has elapsed.

```asm
void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);
```
FD_ZERO():
This macro clears (removes all file descriptors from) set.
It should be employed as the first step in initializing a
file descriptor set.

FD_SET():
      This macro adds the file descriptor fd to set.  Adding a
      file descriptor that is already present in the set is a
      no-op, and does not produce an error.

FD_CLR():
      This macro removes the file descriptor fd from set.
      Removing a file descriptor that is not present in the set
      is a no-op, and does not produce an error.

FD_ISSET():
      select() modifies the contents of the sets according to
      the rules described below.  After calling select(), the
      FD_ISSET() macro can be used to test if a file descriptor
      is still present in a set.  FD_ISSET() returns nonzero if
      the file descriptor fd is present in set, and zero if it
      is not.

### Poll()
poll() is a system call to wait for one or more file descriptors and is prototyped below: 
```asm
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```
the pollfd struct is prototyped below:
```asm
struct pollfd {
       int   fd;         /* file descriptor */
       short events;     /* requested events */
       short revents;    /* returned events */
};
```
Events is a bit mask that specifies the events the application is interested in.
Revents (return events) is filled with the events that actually occurred.

### Kqueue()
The kqueue() system call provides a generic method	of notifying the user
when an event happens or a	condition holds, based on the results of small
pieces of kernel code termed filters. The kqueue() solves the [10ck](https://en.wikipedia.org/wiki/C10k_problem) 
problem which can handles large number of clients at teh same time.

## Sockets
Every UNIX systems Input/Output action is executed by writing or reading the descriptor file. A Socket allows 
communication between two different processes in a client server application framework, therefore it is a way of 
communicating between two computers using file descriptors.

### Socket()
socket() creates an endpoint for communication and returns a file
descriptor that refers to that endpoint. Socket is basically the 
same as open but then for the server endpoint itself. 
```asm
int socket(int domain, int type, int protocol);
```


### Bind()
bind() is used to associate the socket in your program with a port, server address or IPv4/6 address.
```asm
int bind(int sockfd, struct sockaddr *my_addr, int addrlen)
```
sockfd is the socket file descriptor returned by socket(). my_addr is a pointer to a struct sockaddr that contains
information about your address, namely, port and IP address. addrlen is the length in bytes of that address. 

### Listen()
if you don’t want to connect to a remote host, you must proceed in two steps; first you listen() then accept().
Listen is there to monitor a response from the server. 
```asm
 int listen(int sockfd, int backlog); 
```

### Accept()
The accept() system call is used with connection-based socket
types (SOCK_STREAM, SOCK_SEQPACKET).  It extracts the first
connection request on the queue of pending connections for the
listening socket, sockfd, creates a new connected socket, and
returns a new file descriptor referring to that socket. Accept is similar function but then to
create a fd from the client side.
```asm
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

### Send()
The send() function is used to transmit a message to another socket. The send() call can only be used when
the socket is in connected state. Send is in the end write operation to the socket fd
```asm
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

### Recv()
Calls from the function recv() are used to receive messages from a socket. 
Recieve is in the end a read operation on a socket fd.
```asm
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

### Setsockopt()
You can configure sockets to a different setting. Type 'man 7 socket' to see what options there are for 
configuration.
```asm
int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
```

### Getprotobyname()
The getprotobyname() function returns a protoent structure for the entry from the database that matches the protocol 
name. A connection is opened to the database if necessary.
```asm
struct protoent *getprotobyname(const char *name);
```

### Getsockname()
getsockname() returns the current address to which the socket
sockfd is bound, in the buffer pointed to by addr.  The addrlen
argument should be initialized to indicate the amount of space
(in bytes) pointed to by addr.  On return it contains the actual
size of the socket address.
```asm
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

### Fcntl()
In this project we are allowed to use the fcntl function as following: fcntl(fd, F_SETFL, O_NONBLOCK).
