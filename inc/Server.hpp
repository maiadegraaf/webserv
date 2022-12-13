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
#include <sys/event.h>
#include "webserv.h"
#include "Config.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Client.hpp"
#include "WSException.hpp"
#include "Location.hpp"


class Config;

class Server
{
	/* *******************
	 * (Con/De)structors *
 	* *******************/
	public:
		Server(Config *conf);
		Server( const Server& rhs)											{ *this = rhs; }
		~Server()															{ }
		Server& operator=( const Server &rhs);

	/* ************
	 * Attributes *
	 * ************/
	private:
		int						_sockFd,
								_kq,
								_len,
								_acceptFd;
		sockaddr_in				_servAddr,
								_client_addr;
		Config					*_conf;
		struct kevent			_changeEvent[2];
		map<string, string> 	_contentType;
		map<string, Location>	_location;
		bool 					_closeConnection;
		size_t 					_maxSize;

	/* *********
 	* Setters *
 	* *********/
	public:
		void		setCloseConnection(bool Bool)					{ this->_closeConnection = Bool; }
		void 		setMaxSize( size_t newMaxSize )					{ this->_maxSize = newMaxSize; }

	/* *********
 	* Getters *
 	* *********/
	public:
		bool 						getCloseConnection() const					{ return this->_closeConnection; }
		size_t 						getMaxSize() const							{ return this->_maxSize; }
		int							getSockFd()	const							{ return this->_sockFd; }
		int							getAcceptFd() const							{ return this->_acceptFd; }
		const map<string, Location> &getLocation() const						{ return this->_location; }
		const map<string, string> 	&getContentType() const						{ return this->_contentType; }
		int 						getKq() const								{ return this->_kq; }

	/* **************
 	* Functionality *
 	* ***************/
	public:
		// Server.cpp
		void		setup();
		void 		setupSockFd();
		void		setupSocketOpt();
		void 		setupNonBlock();
		void		setAddr();
		void 		bindSocket();
		void 		listenSocket();
		void		output();

		// ServerKq.cpp
		void		setupKq(int kq);
		void 		setKqRead();
		void 		setKqWrite();
		void		clientNewAcceptFd(int eventFd);
		void		bindServerAcceptFdWithClient();

};


#endif
