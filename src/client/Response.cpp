#include "Response.hpp"
#include "Server.hpp"

Response::Response(string errorMessage, int newSockFD, string contentType)
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath("www/error/") {
	stringstream	ss;
	string			tmp;
	ss << errorMessage;
	getline(ss, tmp, ' '); // filepath moet nog hierin komen als defualt anders wordt gemaakt van de errors
	_filePath.append(tmp); // check voor nieuwe error
	_filePath.append(".html");
	setFileSize(fileSize(_filePath.c_str()));
	appendToHeadNL(errorMessage);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHead("\r\n");
}

Response::Response(string filePath, string message, string contentType, int newSockFD, off_t fileSize) // keep alive must be there
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath(filePath), _fileSize(fileSize) {
	appendToHeadNL(message);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
    appendToHeadNL("X-Powered-By: PHP/7.2.17");
	appendToHead("\r\n");
}

Response&	Response::operator=( const Response& rhs )
{
	this->_sockFD = rhs._sockFD;
	this->_fileSize = rhs._fileSize;
	this->_head = rhs._head;
	this->_filePath = rhs._filePath;
	return *this;
}
 
// Output
void Response::output() {
	std::cout << "sockFD : " << _sockFD << std::endl;
	cout << "head : " << _head << endl;
	std::cout << "fileSize : " << _fileSize << std::endl;
}

bool Response::sendResponse() {
	int read = open(getFilePath().c_str(), O_RDONLY);
	send(getSockFD(), _head.c_str(), _head.size(), 0);
//    fork();
//    execv("usr/bin/php", )
	if (sendfile(read, getSockFD(), 0, &_fileSize, NULL, 0) < 0) {
		cerr << "send() failed" << endl;
		return close(read), false;
	}
	close(read);
	return true;
}
