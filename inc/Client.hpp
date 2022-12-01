#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <string>
#include <map>
#include "Response.hpp"
#include "Request.hpp"
#include "WSException.hpp"


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
			: _sockFD(0), _strRequest(""), _maxSize(0) 										{}
		Client(int newSockFD, map<string, string> newLocation, \
			map<string, string> newContentType, size_t newMaxSize);


	/* ************
	 * Attributes *
	 * ************/
	private:
		int 								_sockFD,
											_len;
		map<string, string> 				_location,
											_contentType; // maybe pointer to reduce space usage;
		string 								_strRequest;
		size_t								_maxSize;
		bool 								_closeConnection; // this can be probably removed
		// Add in the event struct if it is usefull ??
		// Add in the 5 string variables here from handleRequest() and put in getters/setters as well for them

	/* *********
 	* Getters *
 	* *********/
	public:
		int			getSockFD()																	{ return this->_sockFD; }
		string		getLocation(string key)														{ return this->_location[key]; }
		string		getContentType(string key)													{ return this->_contentType[key]; }
		string		getStrRequest()																{ return this->_strRequest; }
		bool 		getCloseConnection()														{ return this->_closeConnection; } // this can probably be removed
		size_t 		getMaxSize()																{ return this->_maxSize; }

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
		void 		handleRequest(Request clientReq);
		void		handleResponse(string filePath, string contentType);
		void		handleCGIResponse(string filePath, string contentType);

};

#endif
