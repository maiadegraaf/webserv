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
	private:
		vector<Config>				_config;
		vector<Server>				_server;
		size_t 						_serverSize;
		map<string, string>			_contentType;
		int 						_kq,
									_nrEvents,
									_eventFd;
		map<int, size_t>			_sockFdIdxMap,
									_acceptFdIdxMap;
		struct kevent				_events[2];

	public:
	// Constructor
		WebServ()																	{}
		~WebServ()																	{}
		WebServ(vector<Config> newConfig, map<string, string> newContentType);
		WebServ( const WebServ &rhs);
		WebServ& operator=( const WebServ &rhs);

	// Getters
		int		getKq()																	{ return this->_kq; }
		size_t	getServerSize()															{ return this->_serverSize; }
	// Setters
		void	setKq(int kq)															{ this->_kq = kq; }
		void	output();
		void	runWebServ();
		void	initKq();
//		void 	newEvent( size_t idx );
		void	loopEvent( size_t idx );
		void 	newEvent( );
		void	loopEvent( );
};
 
#endif
