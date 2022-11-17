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
	private:
		int						_fd;
		sockaddr_in				_servAddr;
		Config					*_conf;
		pollfd					_fds[200];
		int						_len;
		int						_nfds;
		int						_newFd;
		map<string, string> 	_contentType;

	public:
		Server(Config *conf);
		Server( const Server& rhs);
		~Server();
		Server& operator=( const Server &rhs);

		void 		creatingPoll();
		void 		newConnection();
		bool		clientRequest(int i, bool *close_conn);
		void		loopFds();
		string		receiveRequest(int i, bool *close_conn);
		void		output();
		void		setup();
		void		setAddr();
		void		closeFds();
		void		run();

	class PageNotFoundException : public exception {
		public:
			const char *what() const throw() {
				return "404 Page Not Found";
			}
	};

}; 
 
#endif
