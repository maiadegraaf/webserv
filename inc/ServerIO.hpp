#ifndef SERVERIO_HPP
#define SERVERIO_HPP
#include <iostream>
#include <string>
#include "Config.hpp"
#include "Server.hpp"

using namespace std;
 
// Class definition 
class ServerIO
{
	/* *******************
	 * (Con/De)structors *
 	* *******************/
	public:
		ServerIO()																	{}
		~ServerIO()																	{}
		ServerIO(vector<Config> newConfig, map<string, string> newContentType, char** envp);
		ServerIO( const ServerIO &rhs);
		ServerIO& operator=( const ServerIO &rhs);

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
        char**                      _envp;

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
		void	runServerIO();
		void	initKq();
		void 	newEvent();
		void	loopEvent();
		void	disconnectClient(void *udata);
		void 	connectNewClient();
		void 	incomingRequest(void *udata);
		void 	outgoingResponse(void *udata);

		void	setupClientRead(Client *client);
		void	setupClientWrite(Client *client);
		void 	setupClientEOF(Client *client);
		void	output();
};

#endif
