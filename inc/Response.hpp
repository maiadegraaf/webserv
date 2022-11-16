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

using namespace std;

// Class definition 
class Response
{
	/* ********************
	 * (Con/De)structor's *
	 * ********************/
	public:
		// Canonical Form
		Response()
			: _sockFD(0), _head(""), _filePath(""), _fileSize(0)					{ }
		~Response()																		{ }
		Response(const Response &rhs) 													{ *this = rhs; }
		Response&	operator=( const Response &rhs);

		// Error & Regular construction
        Response(string errorMessage, int newSockFD, string contentType);
        Response(string filePath, string message, string contentType, \
		int newSockFD, off_t fileSize);

	/* ************
	 * Attributes *
	 * ************/
	private:
		int			_sockFD;
		string		_head;
		string		_filePath;
		off_t 		_fileSize;

	/* *********
	 * Getters *
	 * *********/
	public:
		int		getSockFD()								{ return _sockFD; }
		string	getHead()								{ return _head; }
		string	getFilePath()							{ return _filePath; }
		off_t 	getFileSize()							{ return _fileSize; }

	/* *********
	 * Setters *
	 * *********/
	public:
		void	setSockFD(int newSockFD)						{ this->_sockFD = newSockFD; }
		void	setHead(string newHead)							{ this->_head = newHead; }
		void	setFilePath(string newFilePath)					{ this->_filePath = newFilePath; }
		void	setFileSize(off_t newFileSize)					{ this->_fileSize = newFileSize; }
		void	appendToHead(string newHead)					{ this->_head.append(newHead); }
		void 	appendToHeadNL(string newHead)					{ appendToHead(newHead); this->_head.append("\n"); }
		void 	appendObjectToHead(string type, string name)	{ appendToHead(type); appendToHeadNL(name); }
		void	appendToFilePath(string newPath)				{ this->_filePath.append(newPath); }

	/* ********
	 * Output *
	 * ********/
	public:
		void	output();
		bool	sendResponse();
};
 
#endif
