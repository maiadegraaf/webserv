#include "Response.hpp"

Response::Response(string errorMessage, int newSockFD, string contentType)
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath("www/error/"), _hasBody(true), _sendHeader(false) { // checkout errorpage paths
	stringstream	ss;
	string			tmp;
	ss << errorMessage;
	getline(ss, tmp, ' '); //TODO: filepath moet nog hierin komen als defualt anders wordt gemaakt van de errors
	_filePath.append(tmp); // TODO: check voor nieuwe error
	_filePath.append(".html");
	setFileSize(fileSize(_filePath.c_str()));
	appendToHeadNL(errorMessage);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHead("\r\n");
}

Response::Response(string filePath, string message, string contentType, int newSockFD, off_t fileSize)
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath(filePath), _fileSize(fileSize), _hasBody(true), _sendHeader(false) {
	appendToHeadNL(message);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHead("\r\n");
}

Response&	Response::operator=( const Response& rhs ) {
	this->_sockFD = rhs._sockFD;
	this->_fileSize = rhs._fileSize;
	this->_head = rhs._head;
	this->_filePath = rhs._filePath;
	this->_hasBody = rhs._hasBody;
	this->_sendHeader = rhs._sendHeader;
	return *this;
}
 
// Output
void Response::output() {
	std::cout << "sockFD : " << _sockFD << std::endl;
	cout << "head : " << _head << endl;
	std::cout << "fileSize : " << _fileSize << std::endl;
	std::cout << "filePath : " << _filePath << std::endl;
}

//bool Response::sendResponse() {
//	int read = open(getFilePath().c_str(), O_RDONLY);
//	send(getSockFD(), _head.c_str(), _head.size(), 0);
//	if (sendfile(read, getSockFD(), 0, &_fileSize, NULL, 0) < 0) {
//		cerr << "send() failed " << errno << endl;
//		return close(read), false;
//	}
//	close(read);
//	return true;
//}

void	Response::sendHeader() {
	if (send(getSockFD(), _head.c_str(), _head.size(), 0) < 0)
		perror("send header failed");
	else
		setSendHeader(true);
}

void	Response::sendBody() {
//	cerr << "komonnnnn" << endl;
	int read = open(getFilePath().c_str(), O_RDONLY);
	if (read < 0) {
		perror("sendfile body open fd failed");
		return ;
	}
	if (sendfile(read, getSockFD(), 0, &_fileSize, NULL, 0) < 0) {
		perror("sendfile body failed");
	}
	close(read);
}
