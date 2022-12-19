#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <string>
#include <map>
#include "Response.hpp"
#include "Request.hpp"
#include "WSException.hpp"
#include "Location.hpp"

//class Response;

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
			: _sockFd(-1), _requestBuffer(""), _maxSize(0) 										{}
		Client(int newSockFd, map<string, Location> newLocation, \
			map<string, string> newContentType, size_t newMaxSize);


	/* ************
	 * Attributes *
	 * ************/
	private:
		int 								_sockFd,
											_len;
		map<string, string>					_contentType;
		map<string, Location>		        _location;
		string 								_requestBuffer;
		size_t								_maxSize;
		Request								_request;
		Response							_response;
		bool 								_requestMode; // weg

	/* *********
 	* Getters *
 	* *********/
	public:
		int					getSockFd() const													{ return this->_sockFd; }
		const string		&getContentType(string key)											{ return this->_contentType[key]; }
		Location			&getLocation(string key)											{ return this->_location[key]; }
		string				getRequestBuffer() const											{ return this->_requestBuffer; }
		size_t 				getMaxSize() const													{ return this->_maxSize; }
		bool 				getRequestMode()													{ return this->_requestMode; }

	/* *********
 	* Setters *
 	* *********/
	public:
		void		setSockFD(int newSockFd)													{ this->_sockFd = newSockFd; }
		void 		setRequestMode(bool nBool)													{ this->_requestMode = nBool; }

	/* **************
 	* Functionality *
 	* ***************/
	public:
		void		output();
		bool		requestReceived();
		void 		fillRequestBuffer();
		bool 		recvError(int rc); // even herzien.
		void 		handleRequest();
		void		setResponse(string filePath, string contentType);

		bool 		responseSend();
		void 		resetRequest();

};

#endif
