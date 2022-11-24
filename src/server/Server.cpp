#include "Server.hpp"

Server::Server(Config *conf)
		: _fd(-1), _conf(conf), _closeConnection(false) {
	this->setup();
	this->run();
}

Server&	Server::operator=( const Server& rhs )
{
	(void)rhs;
	return *this;
}

void Server::setup()
{
	int on = 1;

	_contentType = returnContentType();
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
	{
		cerr << "could not create socket (server)" << endl;
		exit(-1);
	}
	if (setsockopt(_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		cerr << "setsockopt() failed" << endl;
		close(_fd);
		exit(-1);
	}
	//makes the socket non blocking
	if (ioctl(_fd, FIONBIO, (char *)&on) < 0) // not allowed
	{
		cerr << "ioctl failed: to make the socket unblocking" << endl;
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
	_len = sizeof(_client_addr);
}

void Server::setAddr()
{
	bzero((char*)&_servAddr, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr.sin_port = htons(_conf->getAddress());
}