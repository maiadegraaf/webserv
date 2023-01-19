#include "CGIResponse.hpp"
#include "stdio.h"


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
	setHead("HTTP/1.1 ");
	setSockFD(newSockFD);
	setFileSize(fileSize);
	appendToHeadNL(message);
	appendObjectToHead("Content-Type: ", contentType);
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
  //cout << "type : " << _type << std::endl;
}

extern char **environ;

bool CGIResponse::exec()
{
	char *split[2];
	string filePath = getFilePath();

	split[0] = new char[getFilePath().length() + 1];
	strcpy(split[0], filePath.c_str());
	split[1] = NULL;
	execve(split[0], split, environ);
	perror("");
	return (EXIT_FAILURE);
}

//void	dup_cmd(int end[2], int fd_in)
//{
//	if (dup2(fd_in, STDIN_FILENO) < 0)
//		;
////		ft_error(4, tools);
//	close(end[0]);
//	if (dup2(end[1], STDOUT_FILENO) < 0)
//		;
////		ft_error(4, tools);
//	close(end[1]);
////	handle_cmd(cmd, tools);
//}
bool CGIResponse::sendResponse() {
	int pid = fork();
	if (pid == 0)
	{
		if (dup2(getSockFD(), STDOUT_FILENO) < 0)
			failure("");
		close(getSockFD());
		exec();
	}
	close(getSockFD());
//	int end[2];
//	pipe(end);
//	int pid = fork();
//	if (pid == 0)
//	{
//		if (dup2(end[0], getSockFD()) < 0)
//			failure("");
//		if (dup2(end[1], STDOUT_FILENO) < 0)
//			failure("");
//		close(end[1]);
//		close(end[0]);
//		exec();
//	}
//	close(end[1]);
//	close(end[0]);
	send(getSockFD(), getHead().c_str(), getHead().size(), 0);
	while(waitpid(pid, NULL, WUNTRACED) != -1);
	return true;
}
