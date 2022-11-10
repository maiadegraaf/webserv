#include "Server.hpp"

Server::Server(){}
 
Server::Server( const Server& rhs)
{
	*this = rhs;
}
 
Server::~Server(){}

Server&	Server::operator=( const Server& rhs )
{
	return *this;
}

void Server::output(){}
 
int Server::setup()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
    {
        std::cerr << "could not create socket (server)" << std::endl;
        exit(-1);
    }
    //makes the socket non blocking
    if (ioctl(_fd, FIONBIO, (char *)&on) < 0)
    {
        std::cerr << "ioctl failed: to make the socket unblocking" << std::endl;
        close(_fd);
        exit(-1);
    }
    this->setAddr();
    if(bind(_fd, (struct sockaddr*) &_servAddr, sizeof(_servAddr)) < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        close(_fd);
        exit(-1);
    }
    if (listen(_fd, 32) < 0)
    {
        cerr << "Listen failed" << endl;
        close(_fd);
        exit(-1);
    }
}

void Server::setAddr()
{
    bzero((char*)&_servAddr, sizeof(_servAddr));
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _servAddr.sin_port = htons(_port);
}