#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <string>
#include <map>
#include "Response.hpp"
#include "Request.hpp"
#include "WSException.hpp"
#include "Location.hpp"


// Class definition
class Client {
	/* *******************
	 * (Con/De)structors *
 	* *******************/
	public:
		// Constructor
		~Client()																				{}
		Client( const Client &rhs)																{ *this = rhs; }
		Client& operator=( const Client &rhs);
		Client()
			: _sockFD(-1), _strRequest(""), _maxSize(0) 										{}
		Client(int newSockFD, map<string, Location> newLocation, \
			map<string, string> newContentType, size_t newMaxSize);


	/* ************
	 * Attributes *
	 * ************/
	private:
		int 								_sockFD,
											_len,
											_read;
		map<string, string>					_contentType;
		map<string, Location>		        _location;
		string								_requestBuffer;
		size_t								_maxSize;
		bool 								_closeConnection;
		Request								_clientRequest;
		Response							_response;

		// this can be probably removed
		// Add in the 5 string variables here from handleRequest() and put in getters/setters as well for them

	/* *********
 	* Getters *
 	* *********/
	public:
		int					getSockFD() const													{ return this->_sockFD; }
		const string		&getContentType(string key)											{ return this->_contentType[key]; }
		Location			&getLocation(string key)											{ return this->_location[key]; }
		string				getStrRequest() const												{ return this->_strRequest; }
		bool 				getCloseConnection() const											{ return this->_closeConnection; } // this can probably be removed
		size_t 				getMaxSize() const													{ return this->_maxSize; }

	/* *********
 	* Setters *
 	* *********/
	public:
		void		setSockFD(int newSockFD)													{ this->_sockFD = newSockFD; }
		void		setStrRequest(string newRequest)											{ this->_strRequest = newRequest; }
		void		setCloseConnection(bool Bool)												{ this->_closeConnection = Bool; } // this can probably be removed

	/* **************
 	* Functionality *
 	* ***************/
	public:
		void		output();
		void 		clientRequest();
		string		receiveStrRequest();
		bool 		recvError(int rc);
		void 		handleRequest(Request clientReq);
		void		handleResponse(string filePath, string contentType);
		void		handleCGIResponse(string filePath, string contentType);

};

#endif
