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
#include "webserv.h"
#include "Config.hpp"
#include "Response.hpp"
#include "Request.hpp"

class Config;

class Server
{
	/* *******************
	 * (Con/De)structors *
 	* *******************/
	public:
		Server(Config *conf);
		Server( const Server& rhs)											{ *this = rhs; }
		~Server()															{ this->closeFds(); }
		Server& operator=( const Server &rhs);

	/* ************
	 * Attributes *
	 * ************/
	private:
		int						_fd;
		sockaddr_in				_servAddr;
		Config					*_conf;
		pollfd					_fds[200];
		int						_len;
		int						_nfds;
		int						_newFd;
		map<string, string> 	_contentType;
		bool 					_closeConnection;

	/* *********
 	* Setters *
 	* *********/
	public:
		void		setAddr();
		void		setCloseConnection(bool Bool)			{ this->_closeConnection = Bool; }


	/* *********
 	* Getters *
 	* *********/
	public:
		bool 		getCloseConnection()					{ return this->_closeConnection; }

	/* **************
 	* Functionality *
 	* ***************/
	public:
		// Server.cpp
		void		output()													{}
		void		setup();
		// ServerRun.cpp
		void		run();
		void 		newConnection();
		void 		creatingPoll();
		void		loopFds();
		void		closeFds();
		// ClientResponse.cpp
		bool		clientRequest(int i);
		string		receiveStrRequest(int i);
		bool 		handleRequest(Request clientReq, int i);
		void 		handleResponse(string filePath, string contentType, int i);
		void 		handleCGIResponse(string filePath, string contentType, int i);

	/* ************
	 * Exceptions *
	 * ************/
	public:
		class PageNotFoundException : public exception {
			public:
				const char *what() const throw() {
					return "404 Page Not Found";
				}
			};
}; 
 
#endif
