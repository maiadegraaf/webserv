#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include <iostream>
#include <string>
#include "Config.hpp"
#include "Server.hpp"

using namespace std;
 
// Class definition 
class WebServ
{
	/* *******************
	 * (Con/De)structors *
 	* *******************/
	public:
		WebServ()																	{}
		~WebServ()																	{}
		WebServ(vector<Config> newConfig, map<string, string> newContentType);
		WebServ( const WebServ &rhs);
		WebServ& operator=( const WebServ &rhs);

	/* ************
 	* Attributes *
 	* ************/
	private:
		vector<Config>				_config;
		vector<Server>				_server;
		size_t 						_serverSize;
		map<string, string>			_contentType;
		int 						_kq,
									_nrEvents,
									_eventFd;
		map<int, size_t>			_sockFdIdxMap;
		struct kevent				_events[2];

	/* *********
 	* Getters *
 	* *********/
	public:
		int		getKq()																	{ return this->_kq; }
		size_t	getServerSize()															{ return this->_serverSize; }

	/* **************
 	* Functionality *
 	* ***************/
	public:
		void	runWebServ();
		void	initKq();
		void 	newEvent();
		void	loopEvent();
		void	disconnectClient(void *udata);
		void 	connectNewClient();
		void 	incomingRequest(void *udata);
		void	output();
};

#endif
