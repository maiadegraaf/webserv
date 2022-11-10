#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <string>


class Server
{
private:
    int _fd;
    sockaddr_in _servAddr;
public:
	Server(); 
	Server( const Server &rhs); 
	~Server(); 
	Server& operator=( const Server &rhs);
	void output(); 
}; 
 
#endif
