#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <string>
#include <map>
#include "Response.hpp"
#include "Request.hpp"
#include "WSException.hpp"
#include "Location.hpp"
#include "CGIResponse.hpp"

// class Response;

typedef enum e_clientMode {
	request,
	response
}	clientMode;

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
		vector< map<string, string> >		_headerMultipart;
		string 								_requestBuffer;
		vector< vector<string> >			_postContent;
		size_t								_maxSize;
		Request								_request;
		Response							_response;
		clientMode							_clientMode;

	/* *********
 	* Getters *
 	* *********/
	public:
		int					getSockFd() const													{ return this->_sockFd; }
		const string		&getContentType(string key)											{ return this->_contentType[key]; }
		Location			&getLocation(string key)											{ return this->_location[key]; }
		string				getRequestBuffer() const											{ return this->_requestBuffer; }
		size_t 				getMaxSize() const													{ return this->_maxSize; }
		bool 				getClientMode()														{ return this->_clientMode; }
		vector< vector<string> >	getPostContent()											{ return this->_postContent; }

	/* *********
 	* Setters *
 	* *********/
	public:
		void		setSockFD(int newSockFd)													{ this->_sockFd = newSockFd; }
		void 		setClientMode(clientMode mode)												{ this->_clientMode = mode; }
		void 		setPostContent(string input, int i);
		void		setHeaderMultipartValue(string key, string value, int i)	{ _headerMultipart[i][key] = value; }

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
		void 		clientRequest();
		string		receiveStrRequest();
		void		handleGetRequest(string file, string filepath);
		void		handlePostRequest(string file, string filepath, Request clientReq);
		void		handleDeleteRequest();
		void		handleResponse(string filePath, string contentType);
		void		handleCGIResponse(const string& filePath, const string& contentType, const string& file);
		void		parsePostPlainRequest(Request clientReq);
		void 		parsePostWwwRequest(Request clientReq);
		void 		parsePostMultipartRequest(Request clientReq);
		void		parseHeaderMultipart(string *req, stringstream *ss, int content_nb, Request clientReq, bool *endOfReq);
		void 		makeMapOfMultipartHeader(string tmp, int content_nb);
		void		createFileStorePost(int i);
		void 		decryptWwwForm(string &data);

};

#endif
