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
		Client(int newSockFd, map<string, Location> newLocation, string newRoot, map<int, string> newErrorPages,  map<string, string> newContentType, \
	size_t newMaxSize, vector<string> hostNames);

	/* ************
	 * Attributes *
	 * ************/
	private:
		int 								_sockFd,
											_len;
		map<string, string>					_contentType;
		map<string, Location>		        _location;
		vector< map<string, string> >		_headerMultipart;
		string 								_requestBuffer,
											_root;
		map<int, string>					_errorPages;
		vector< vector<string> >			_postContent;
		size_t								_maxSize;
		Request								_request;
		Response							_response;
		clientMode 							_clientMode;
		vector<string>						_hostNames;
		bool								_endOfRequest,
											_isParsing; // weg

	/* *********
 	* Getters *
 	* *********/
	public:
		int					getSockFd() const													{ return this->_sockFd; }
		const string		&getContentType(const string &key)									{ return this->_contentType[key]; }
		Location			&getLocation(const string &key)										{ return this->_location[key]; }
		string				getRequestBuffer() const											{ return this->_requestBuffer; }
		size_t 				getMaxSize() const													{ return this->_maxSize; }
		vector< vector<string> >	getPostContent()											{ return this->_postContent; }
		string				getRoot() const														{ return this->_root; }
		string				getErrorPageValue(const int &key)									{ return this->_errorPages[key]; }
		clientMode			&getClientMode()														{ return this->_clientMode; }

	/* *********
 	* Setters *
 	* *********/
	public:
		void		setSockFD(int newSockFd)													{ this->_sockFd = newSockFd; }
		void 		setEndOfRequest(bool nBool)													{ this->_endOfRequest = nBool; }
		void 		setIsParsing(bool nBool)													{ this->_isParsing = nBool; }
		void		setHeaderMultipartValue(string key, string value, int i)					{ this->_headerMultipart[i][key] = value; }
		void 		setClientMode(clientMode mode)												{ this->_clientMode = mode; }

	/* **************
 	* Functionality *
 	* ***************/
	public:
		void		output();
		void		requestReceived(char** envp);
		void 		fillRequestBuffer();
		void 		handleRequest(char** envp);
		void		setResponse(string filePath, string contentType);
		void 		responseSend();
		void 		resetRequest();
		void		handleGetRequest(string filepath);
		void		handlePostRequest(const string filepath);
		void		handleDeleteRequest(string filepath, char** envp);
		void		handleCGIResponse(string filePath, const string& contentType, char** envp);
		void		parsePostPlainRequest();
		void 		parsePostWwwRequest();
		void 		parsePostMultipartRequest();
		void		parseHeaderMultipart(string *req, stringstream *ss, int content_nb, bool *endOfReq);
		void 		makeMapOfMultipartHeader(string tmp, int content_nb);
		void		createFileStorePost(int i);
		void 		decryptWwwForm(string &data);
		void		setPostResponse(string contentType);

    Location handleMethod();
};

#endif
