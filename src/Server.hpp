#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <poll.h>
#include <sys/ioctl.h>
#include "config.hpp"

#define TRUE        1
#define FALSE       0

class Server
{
private:
    int _fd;
    sockaddr_in _servAddr;
    config *_conf;
    pollfd _fds[200];
    int     _len;
    int     _nfds;
    int     _newFd;

public:
	Server(config *conf);
	Server( const Server &rhs); 
	~Server(); 
	Server& operator=( const Server &rhs);
	void output();
    void setup();
    void setAddr();
    void closeFds();
    void run();
    std::ifstream::pos_type filesize(const char* filename);

}; 
 
#endif
