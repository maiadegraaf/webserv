#ifndef RESPONSE_H
#define RESPONSE_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sys/socket.h>
#include <sstream>
#include <map>
#include <fcntl.h>
#include "webserv.h"
#include "Utils.h"
#include "WSException.hpp"


typedef enum e_clientMode {
	request,
	response,
	disconnect
} clientMode;

//using namespace std;

class Response
{
	/* ********************
	 * (Con/De)structor's *
	 * ********************/
	public:
		// Canonical Form
		Response()
			: _sockFD(0), _head(""), _filePath(""), _fileSize(0),
			_hasBody(false), _sendHeader(false)											{ }
		~Response()																		{ if (_fileFd != -1) close(_fileFd); }
		Response(const Response &rhs) 													{ *this = rhs; }
		Response&	operator=( const Response &rhs);

		// Error & Regular construction
		Response(string message, string contentType, int newSockFD, off_t fileSize);
		Response(string errorMessage, string errorFilePath, int newSockFD, string contentType);
		Response(const string& filePath, const string& message, const string& contentType, int newSockFD, off_t fileSize);
//		Response(const Request &request, int newSockFD); // new

	/* ************
	 * Attributes *
	 * ************/
	private:
		int			_sockFD,
					_fileFd;
		string		_head;
		string		_filePath;
		off_t 		_fileSize,
					_offset;
		string 		_contentType;
		bool 		_hasBody, // new
					_sendHeader; // new

	/* *********
	 * Getters *
	 * *********/
	public:
		int				getSockFD()								{ return _sockFD; }
		string			getHead()								{ return _head; }
		string			getFilePath()							{ return _filePath; }
		off_t 			getFileSize()							{ return _fileSize; }
		bool 			getHasBody()							{ return _hasBody; }
		bool 			getSendHeader()							{ return _sendHeader; }
		const string	&getContentType() const 				{ return _contentType; }
		const	int 	&getFileFd()							{ return _fileFd; }


	/* *********
	 * Setters *
	 * *********/
	public:
		void	setSockFD(int newSockFD)						{ this->_sockFD = newSockFD; }
		void	setHead(string newHead)							{ this->_head = newHead; }
		void	setFilePath(string newFilePath)					{ this->_filePath = newFilePath; }
		void	setFileSize(off_t newFileSize)					{ this->_fileSize = newFileSize; }
		void	appendToHead(string newHead)					{ this->_head.append(newHead); }
		void 	appendToHeadNL(string newHead)					{ this->appendToHead(newHead); this->_head.append("\r\n"); }
		void 	appendObjectToHead(string type, string name)	{ this->appendToHead(type); appendToHeadNL(name); }
		void	appendToFilePath(string newPath)				{ this->_filePath.append(newPath); }
		void 	setHasBody(bool nBool)							{ this->_hasBody = nBool; }
		void 	setSendHeader(bool nBool)						{ this->_sendHeader = nBool; }
		void 	setFileFd(const int &fd)						{ this->_fileFd = fd; }

	/* ****
	* CGI *
	* *****/

	bool exec(char** envp);
	string CGIResponse(char** envp);

	/* ********
	 * Output *
	 * ********/
	public:
		void		output();
		void 		sendHeader();
		clientMode 	sendBody();
//		bool	sendResponse();
//		bool 	sendCGI();

	void setNewHeader(const string& message, const string& contentType);
};
 
#endif
