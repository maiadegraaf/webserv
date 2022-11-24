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
		int						_fd,
								_kq,
								_new_events,
								_len,
								_newFd,
								_event_fd;
		sockaddr_in				_servAddr, _client_addr;
		Config					*_conf;
		struct kevent			_change_event[2], _event[2];
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
		void 		newEvent();
		void 		creatingKqueue();
		void		loopEvent();
		// ClientResponse.cpp
		bool		clientRequest();
		string		receiveStrRequest();
		bool 		handleRequest(Request clientReq);
		void 		handleResponse(string filePath, string contentType);
		void 		handleCGIResponse(string filePath, string contentType);

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

typedef struct s_udata {

}				t_udata;


#endif
