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
	cerr << "\033[1;31m" << to_string(getFileSize()) << "\033[0m" << endl;
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
    FILE *fp = fopen(getFilePath().c_str(), "r");
	int buffer = 1024; //chunk size of 1kb
	char filebyte[buffer];
	int32_t readBytes;

//	if (_fileSize > 0)
//	{
		while((readBytes = fread(filebyte, 1, buffer, fp)) > 0) {
//			cerr << readBytes << endl;
			send(getSockFD(), filebyte, readBytes, 0);
		}
//	}
	cerr << "DONE SENDING." << endl;
//		while (_fileSize > 0)
//		{
//			_fileSize -= offset;
//			if (sendfile(read, getSockFD(), offset, &_fileSize, NULL, 0) < 0)
//				perror("sendfile body failed");
//			offset += 200000;
//		}

	fclose(fp);
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
	perror("");
	return (EXIT_FAILURE);
}

string Response::CGIResponse(char **envp)
{
//	string	tmp;
//	int		front;
//	if (_contentType == "php")
//		front = 4;
//	else
//		front = 5;
//	string subFilePath = getFilePath().substr(front, getFilePath().length());
//	subFilePath = subFilePath.substr(0, subFilePath.find_last_of('/'));
//	tmp =  subFilePath + "/tmpFile" + ".html";
	string tmp = "obj/.tmpfile.html";
	char *filename = const_cast<char *>(tmp.c_str());
	cout << getFilePath() << endl;
	cout << "filename = " << filename << endl;
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
