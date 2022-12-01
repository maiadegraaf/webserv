#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <string>
#include <map>
#include "Response.hpp"
#include "Request.hpp"

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
			: _sockFD(0), _strRequest("") 													{}
		Client(int newSockFD, map<string, string> newLocation, \
			map<string, string> newContentType);

	/* ************
	 * Attributes *
	 * ************/
	private:
		int 								_sockFD,
											_len;
		map<string, string> 				_location,
											_contentType; // maybe pointer to reduce space usage;
		string 								_strRequest;
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
		void		handleGetRequest(string file, string filepath);
		void		handlePostRequest(string file, string filepath, Request clientReq);
		void		handleDeleteRequest();
		void		handleResponse(string filePath, string contentType);
		void		handleCGIResponse(string filePath, string contentType);

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
