#include "Response.hpp"
// Constructor initializes attributes to 0 by default
Response::Response(string errorMessage, int newSockFD)
	: _sockFD(newSockFD) {


	_head.append("HTTP/1.1 ");
	_head.append(errorMessagemessage);
	_head.append("\r\n\r");

}

Response::Response(string file, string message, int newSockFD)
	: _sockFD(newSockFD) {
	(void)file, (void)messsage;
}

Response&	Response::operator=( const Response& rhs )
{
	this->_sockFD = rhs._sockFD;
	this->_file = rhs._file;
	this->_fileSize = rhs._fileSize;
	this->_head = rhs._head;
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

}


