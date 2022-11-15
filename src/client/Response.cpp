#include "Response.hpp"
#include "Server.hpp"

Response::Response(string errorMessage, int newSockFD)
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath("www/error/") {
	stringstream	ss;
	string			tmp;
	map<string, string> gContentType = returnContentType();
	ss << errorMessage;
	getline(ss, tmp, ' '); // filepath moet nog hierin komen als defualt anders wordt gemaakt van de errors
	_filePath.append(tmp);
	_filePath.append(".html");

	setFileSize(Server::fileSize(_filePath.c_str()));
	string size = to_string(getFileSize());
	appendToHeadNL(errorMessage);
	appendToHead("Content-Type: ");
	appendToHeadNL(gContentType["html"]);
	appendToHead("Content-Length: ");
	appendToHeadNL(size);
	appendToHead("\r\n\r");
}

Response::Response(string filePath, string message, string contentType, int newSockFD, off_t fileSize) // keep alive must be there
	: _sockFD(newSockFD), _head("HTTP1/1 "), _filePath(filePath), _fileSize(fileSize) {
	appendToHeadNL(message);
	appendToHead("Content-Type: ");
	appendToHeadNL(contentType);
	appendToHead("Content-Length: ");
	appendToHeadNL(to_string(getFileSize()));
	appendToHead("\r\n\r");
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
  std::cout << "head : " << _head << std::endl; 
  std::cout << "file : " << _file << std::endl; 
  std::cout << "fileSize : " << _fileSize << std::endl; 
}

// functionality
void Response::sendResponse() {
	int read = open(getFilePath().c_str(), O_RDONLY);
	send(getSockFd(), _head.c_str(), _head.size(), 0);
	sendfile(read, getSockFd(), 0, &_fileSize, NULL, 0); // put the rest checkups here for sending the files
	close(fd);
//	if (sendfile(read, _fds[i].fd, 0, &_len, NULL, 0) < 0)
//		{
//			cerr << "send() failed" << endl;
//			close_conn = true;
//			break; // break function needs to work
//		}
//		if (close_conn)
//		{
//			close(_fds[i].fd);
//			_fds[i].fd = -1;
//		}
}
