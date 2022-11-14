#ifndef RESPONSE_H
#define RESPONSE_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sys/socket.h>
//#include <sys/sendfile.h>
#include <fcntl.h>
#include "webserv.h"

using namespace std;

// Class definition 
class Response
{
	// Constructor
	public:
		Response()
			: _sockFD(0), _head(""), _filePath(""), _fileSize(0)	{ }
		~Response()											{ }
		Response(const Response &rhs) 									{ *this = rhs; }
        Response(const char *errorMessage, int newSockFD);
        Response(string filePath, string message, string contentType, int newSockFD);
        Response(string errorMessage, int newSockFD);
		Response(string message, string file, int newSockFD);
		Response&	operator=( const Response &rhs);

	// Attributes
	private:
		int			_sockFD;
		string		_head;
		string		_filePath;
		size_t		_fileSize;
		ifstream	_file;

	// Getters
	public:
		int		getSockFD()								{ return _sockFD; }
		string	getHead()								{ return _head; }
		string	getFilePath()							{ return _filePath; }
		size_t	getFileSize()							{ return _fileSize; }

	// Setters
	public:
		void	setSockFD(int newSockFD)				{ this->_sockFD = newSockFD; }
		void	setHead(string newHead)					{ this->_head = newHead; }
		void	setFile(string newFilePath)				{ this->_filePath = newFilePath; }
		void	setFileSize(size_t newFileSize)			{ this->_fileSize = newFileSize; }

	// Output
	public:
		void	output();

	// Functionality
	public:
		void	sendResponse();
		void	appendToHead(string newHead)			{ this->_head.append(newHead); }
		void	appendToFilePath(string newPath)		{ this->_filePath.append(newPath); }
}; 
 
#endif
