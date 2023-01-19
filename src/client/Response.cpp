#include "Response.hpp"

Response::Response(string errorMessage,string errorFilePath, int newSockFD, string contentType)
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath(errorFilePath), _hasBody(true), _sendHeader(false) {
	setFileSize(fileSize(_filePath.c_str()));
	appendToHeadNL(errorMessage);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHead("\r\n");
}

Response::Response(const string& filePath, const string& message, const string& contentType, int newSockFD, off_t fileSize)
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath(filePath), _fileSize(fileSize), _contentType(contentType), _hasBody(true), _sendHeader(false) {
	appendToHeadNL(message);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHead("\r\n");
}

Response::Response(string message, string contentType, int newSockFD, off_t fileSize)
		: _sockFD(newSockFD), _head("HTTP/1.1 "), _fileSize(fileSize), _hasBody(false), _sendHeader(false) {
	appendToHeadNL(message);
	(void)contentType;
	appendObjectToHead("Content-Length: ", "29");
	appendToHead("\r\n");
	appendObjectToHead("content uploaded successfully", "\r\n");
	appendToHead("\r\n");
}

Response&	Response::operator=( const Response& rhs ) {
	this->_sockFD = rhs._sockFD;
	this->_fileSize = rhs._fileSize;
	this->_head = rhs._head;
	this->_filePath = rhs._filePath;
	this->_hasBody = rhs._hasBody;
	this->_sendHeader = rhs._sendHeader;
	this->_contentType = rhs._contentType;
	return *this;
}

void Response::setNewHeader(const string& message, const string& contentType)
{
	setHead("HTTP/1.1");
	appendToHeadNL(message);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHead("\r\n");
}

// Output
void Response::output() {
	cout << "sockFD : " << _sockFD << std::endl;
	cout << "head : " << _head << endl;
	cout << "fileSize : " << _fileSize << std::endl;
	cout << "filePath : " << _filePath << std::endl;
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
	cerr << "++++++++++++ SEND BODY ++++++++++++" << endl;
	cerr << getFilePath() << endl;
    cerr << "+++++++++++++++++++++++++++++++++++" << endl;
    int read = open(getFilePath().c_str(), O_RDONLY);
	cerr << read << endl;
	if (sendfile(read, getSockFD(), 0, &_fileSize, NULL, 0) < 0) {
		perror("sendfile body failed");
	}
	close(read);
}

extern char **environ;

bool Response::exec()
{
	char **split = splitStr(getFilePath());
	execve(split[0], split, environ);
	perror("");
	return (EXIT_FAILURE);
}

string Response::CGIResponse()
{
	string tmp;
	if (_contentType == "php")
	{
		string subFilePath = getFilePath().substr(0, getFilePath().find_last_of('.') - 1);
		tmp =  subFilePath + "tmpFile" + ".html";
	}
	else
		tmp = "deleted_file.html";
	cerr << tmp << endl;
	char *filename = const_cast<char *>(tmp.c_str());
	int	fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fd < 0)
		failure("");
	int pid = fork();
	if (pid == 0)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
			failure("");
		close(fd);
		exec();
	}
	close(fd);
	while(waitpid(pid, NULL, WUNTRACED) != -1);
	return tmp;
}
