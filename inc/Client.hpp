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
			: _sockFD(-1), _strRequest(""), _maxSize(0) 										{}
		Client(int newSockFD, map<string, string> newLocation, \
			map<string, string> newContentType, size_t newMaxSize);


	/* ************
	 * Attributes *
	 * ************/
	private:
		int 								_sockFD,
											_len;
		map<string, string> 				_location,
											_contentType;// maybe pointer to reduce space usage;
		vector< map<string, string> >		_headerMultipart;
		vector< vector<string> >			_postContent;
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
		vector< vector<string> >	getPostContent()											{ return _postContent; }
	/* *********
 * Setters *
 * *********/
	public:
		void		setSockFD(int newSockFD)													{ this->_sockFD = newSockFD; }
		void		setStrRequest(string newRequest)											{ this->_strRequest = newRequest; }
		void		setCloseConnection(bool Bool)												{ this->_closeConnection = Bool; } // this can probably be removed
		void 		setPostContent(string input, int i);
		void		setHeaderMultipartValue(string key, string value, int i)	{ _headerMultipart[i][key] = value; }


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
		void		parsePostPlainRequest(Request clientReq);
		void 		parsePostWwwRequest(Request clientReq);
		void 		parsePostMultipartRequest(Request clientReq);
		void		parseHeaderMultipart(string *req, stringstream *ss, int content_nb, Request clientReq, bool *endOfReq);
		void 		makeMapOfMultipartHeader(string tmp, int content_nb);
		void		createFileStorePost(int i);
		void 		decryptWwwForm(string &data);

};

#endif
