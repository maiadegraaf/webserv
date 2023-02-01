#include "Response.hpp"

Response::Response(string errorMessage,string errorFilePath, int newSockFD, string contentType)
	: _sockFD(newSockFD), _fileFd(-1), _head("HTTP/1.1 "), _filePath(errorFilePath), _offset(0), _hasBody(true), _sendHeader(false) {
	setFileSize(fileSize(_filePath.c_str()));
	appendToHeadNL(errorMessage);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHead("\r\n");
}

Response::Response(const string& filePath, const string& message, const string& contentType, int newSockFD, off_t fileSize)
	: _sockFD(newSockFD), _fileFd(-1), _head("HTTP/1.1 "), _filePath(filePath), _fileSize(fileSize), _offset(0), _contentType(contentType), _hasBody(true), _sendHeader(false) {
	appendToHeadNL(message);
	appendObjectToHead("Content-Type: ", contentType);
	appendObjectToHead("Content-Length: ", to_string(getFileSize()));
	appendToHead("\r\n");
}

Response::Response(string message, string contentType, int newSockFD, off_t fileSize)
		: _sockFD(newSockFD), _fileFd(-1), _head("HTTP/1.1 "), _fileSize(fileSize), _offset(0), _contentType(contentType),_hasBody(false), _sendHeader(false) {
	appendToHeadNL(message);
	appendObjectToHead("Content-Length: ", "29");
	appendToHead("\r\n");
	appendObjectToHead("content uploaded successfully", "\r\n");
	appendToHead("\r\n");
}

Response&	Response::operator=( const Response& rhs ) {
	this->_sockFD = rhs._sockFD;
	this->_fileFd= rhs._fileFd;
	this->_head = rhs._head;
	this->_offset = rhs._offset;
	this->_fileSize = rhs._fileSize;
	this->_filePath = rhs._filePath;
	this->_contentType = rhs._contentType;
	this->_hasBody = rhs._hasBody;
	this->_sendHeader = rhs._sendHeader;
	return *this;
}

void Response::setNewHeader(const string& message, const string& contentType) {
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

bool	Response::sendHeader() {
	if (send(getSockFD(), _head.c_str(), _head.size(), 0) < 0) {
		perror("send header failed");
		return false;
	}
	setSendHeader(true);
	return true;
}

clientMode 	Response::sendBody() {
	off_t	len;
	if (this->getFileFd() < 0) {
		int tmpFd = open(this->getFilePath().c_str(), O_RDONLY);
		if (tmpFd < 0) {
			perror("open file failed");
			return disconnect;
		}
		this->setFileFd(tmpFd);
	}
	if (sendfile(this->getFileFd(), this->getSockFD(), _offset, &len, NULL, 0) < 0) {
		if (len + _offset < _fileSize) {
			_offset += len;
			return response;
		}
		else {
			perror("sendbody failed in sendfile");
			return disconnect;
		}
	}
	close(this->getFileFd());
	this->setFileFd(-1);
	return request;
}

bool Response::exec(char **envp)
{
	char **split = vectorToArr(splitStr(getFilePath(), " ?"));
    if (!access(split[0], F_OK))
        execve(split[0], split, envp);
    vector<string> paths = parse_envp(envp);
    for (size_t i = 0; i < paths.size(); i++)
    {
        string cmd = paths[i] + split[0];
        if (!access(cmd.c_str(), F_OK))
            execve(cmd.c_str(), split, envp);
    }
	perror("exec fail");
	return (EXIT_FAILURE);
}

string Response::CGIResponse(char **envp)
{
	string tmp = "obj/.tmpfile.html";
	char *filename = const_cast<char *>(tmp.c_str());
	int	fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fd < 0)
	{
		perror("CGI: ");
		return("");
	}
	int pid = fork();
	if (pid == 0)
	{
		if (_contentType == "php" && dup2(fd, STDOUT_FILENO) < 0)
			failure("");
		close(fd);
		exec(envp);
	}
	close(fd);
	while(waitpid(pid, NULL, WUNTRACED) != -1);
	return tmp;
}
