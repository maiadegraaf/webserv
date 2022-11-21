#include "CGIResponse.hpp"

// Constructor initializes attributes to 0 by default 
CGIResponse::CGIResponse()
	: _type(0)
{

}
 
CGIResponse::CGIResponse( const CGIResponse& rhs)
{
	*this = rhs;
}
 
CGIResponse::CGIResponse(const string& newType, const string& filePath, const string& message,
						 const string& contentType, int newSockFD, off_t fileSize)
	: _type(newType)
{
	setFilePath(filePath);
	setHead("HTTP/1.1");
	setSockFD(newSockFD);
	setFileSize(fileSize);
	appendToHeadNL(message);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHeadNL("X-Powered-By: PHP/7.2.17");
	appendToHead("\r\n");
}

CGIResponse::CGIResponse(const string& newType) : _type(newType) {}

CGIResponse::~CGIResponse()
{

}

CGIResponse&	CGIResponse::operator=( const CGIResponse& rhs )
{
	_type = rhs._type;
	return *this;
}

// Getters
string CGIResponse::getType() { return _type; }

// Setters
void CGIResponse::setType(string newType) { _type = newType; }

// Output
void CGIResponse::output()
{
  std::cout << "type : " << _type << std::endl;
}

extern char **environ;

bool CGIResponse::exec()
{
	char *split[2];

	getenv("PATH");

	split[0] = new char[getFilePath().length() + 1];
	execve(split[0], split, environ);
	return (EXIT_FAILURE);
}

