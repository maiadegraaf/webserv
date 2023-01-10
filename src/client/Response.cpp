#include "Response.hpp"

Response::Response(string errorMessage, int newSockFD, string contentType)
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath("www/error/"), _contentType(contentType), _hasBody(true), _sendHeader(false) { // checkout errorpage paths
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
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath(filePath), _fileSize(fileSize), _contentType(contentType), _hasBody(true), _sendHeader(false) {
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
	if (sendfile(read, getSockFD(), 0, &_fileSize, NULL, 0) < 0) {
		perror("sendfile body failed");
	}
	close(read);
}

extern char **environ;

bool Response::exec()
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

// cmd handler from minishell
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

bool Response::CGIResponse()
{
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
//	send(getSockFD(), getHead().c_str(), getHead().size(), 0);
	while(waitpid(pid, NULL, WUNTRACED) != -1);
	return true;
}
