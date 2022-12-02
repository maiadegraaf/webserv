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
		sockaddr_in				_servAddr, 
								_client_addr;
		Config					*_conf;
		struct kevent			_change_event[2], 
								_event[2];
		map<string, string> 	_contentType,
								_location;
		bool 					_closeConnection;
		size_t 					_maxSize;

	/* *********
 	* Setters *
 	* *********/
	public:
		void		setAddr();
		void		setCloseConnection(bool Bool)			{ this->_closeConnection = Bool; }
		void 		setMaxSize( size_t newMaxSize )			{ this->_maxSize = newMaxSize; }
		// void 		setupKq(int kq);


	/* *********
 	* Getters *
 	* *********/
	public:
		bool 			getCloseConnection()					{ return this->_closeConnection; }
		size_t 			getMaxSize()							{ return this->_maxSize; }
		int				getSockFd()								{ return this->_fd; }
		struct kevent	*getChangeEvent()						{ return this->_change_event; }
		struct kevent	getChangeEventRead()					{ return this->_change_event[0]; }
		struct kevent	getChangeEventWrite()					{ return this->_change_event[1]; }
		struct kevent	*getEvent()								{ return this->_event; }
		struct kevent	getEventRead()							{ return this->_event[0]; }
		struct kevent	getEventWrite()							{ return this->_event[1]; }
		struct kevent	getEventByIndex( int idx )				{ return this->_event[idx]; }
		// int				getAcceptFd(int eventFd);
		map<string, string> getLocation()						{ return this->_location; }


	/* **************
 	* Functionality *
 	* ***************/
	public:
		// Server.cpp
		void		output()													{}
		void		setup();
		void		setupKq(int kq);
		int			getAcceptFd(int eventFd);
		// ServerRun.cpp
//		void		run();
//		void 		newEvent();
//		void 		creatingKqueue();
//		void		loopEvent();
//		// ClientResponse.cpp
//		bool		clientRequest();
//		string		receiveStrRequest();
//		bool 		handleRequest(Request clientReq);
//		void 		handleResponse(string filePath, string contentType);
//		void 		handleCGIResponse(string filePath, string contentType);

};


#endif
