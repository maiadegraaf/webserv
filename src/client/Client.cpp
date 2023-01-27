#include "Client.hpp"

Client::Client(int newSockFd, map<string, Location> newLocation, string newRoot, map<int, string> newErrorPages,  map<string, string> newContentType, \
size_t newMaxSize)
	: _sockFd(newSockFd), _len(-1), _contentType(newContentType), _location(newLocation), \
	_requestBuffer(""), _root(newRoot), _errorPages(newErrorPages), _maxSize(newMaxSize), _requestMode(true) , _endOfRequest(false), _isParsing(false) {
}

Client&	Client::operator=( const Client& rhs ) {
	this->_sockFd = rhs._sockFd;
	this->_len = rhs._len;
	this->_location = rhs._location;
	this->_contentType = rhs._contentType;
	this->_requestBuffer = rhs._requestBuffer;
	this->_maxSize = rhs._maxSize;
	this->_request = rhs._request;
	this->_response = rhs._response;
	this->_requestMode = rhs._requestMode;
	return *this;
}

// Output
void	Client::output() {
	std::cout << "SockFD : " << _sockFd << std::endl;
	std::cout << "strRequest : " << _requestBuffer << std::endl;
}

void	Client::requestReceived(char** envp) {
	try {
		this->fillRequestBuffer();
		stringstream ss(getRequestBuffer());
		_request.setSS(&ss);
		_request.parseBuffer();
		this->handleRequest(envp);
		_requestBuffer.clear();
	} catch (exception &e) {
		string		tmpMessage(e.what());
		string		filePath(getRoot() + '/');
		int 		errorNr = atoi(tmpMessage.c_str());
		filePath.append(getErrorPageValue(errorNr));
		cout << "this is Filepath :" << filePath << endl;
		Response	error(tmpMessage, filePath, getSockFd(), getContentType("html"));
		_response = error;
		this->resetRequest();
		_requestBuffer.clear();
	}
}

void	Client::fillRequestBuffer() {
	int     rc;
	char    buffer[200];
	string	tmp;

	while (1)
	{
		rc = recv(getSockFd(), buffer, sizeof(buffer), 0);
		if (rc <= 0)
			break ;
		tmp.assign(buffer, rc);
		_requestBuffer.append(tmp);
	}
}

map<e_method, bool> setDefaultMethods()
{
    map<e_method, bool> method;
    method[GET] = false;
    method[POST] = false;
    method[DELETE] = false;
    return method;
}

Location Client::handleMethod()
{
    static map<e_method, bool> method = setDefaultMethods();

    Location location = getLocation(_request.getDir());
    if (!location.isEmpty())
        method = location.getMethod();
    else
        location.setMethod(method);
    return location;
}

void	Client::handleRequest(char** envp) {
	Location	location;
	string		filePath(getRoot());
	string 		path;

//	_request.output();
	location = handleMethod();
	path = location.getIndex();
	if (!path.empty())
		filePath.append(_request.getDir() + '/' + path); // page not foudn exception
	else {
		if (_request.getDir().empty())
			throw WSException::BadRequest();
		filePath.append(_request.getDir()); // Response
	}
	cerr << _request.getDir() << endl;
	location.output();
    if (_request.getMethod() == "GET" && ((!location.isEmpty() && location.getGet()) || location.isEmpty()))
	{
		if ( filePath.find("php") != string::npos ) {
			handleCGIResponse(filePath, "php", envp);
			this->resetRequest();
			return ;
		}
		handleGetRequest(filePath);
	}
	else if (_request.getMethod() == "POST" && location.getPost()) {
		handlePostRequest(filePath);
	}
	else if (_request.getMethod() == "DELETE" && location.getDelete()) {
		handleDeleteRequest(filePath, envp);
	}
    else {
        throw WSException::MethodNotAllowed();
    }
	this->resetRequest();
}

void	Client::handleCGIResponse(string filePath, const string& contentType, char** envp) {
	Response	clientResponse(filePath.insert(0, "php "), "200 OK", contentType, getSockFd(), 0);
	clientResponse.setFilePath(clientResponse.CGIResponse(envp));
	clientResponse.setFileSize(fileSize(clientResponse.getFilePath().c_str()));
	clientResponse.setNewHeader("200 OK", contentType);
	_response = clientResponse;
}

void	Client::setResponse(string filePath, string contentType) {
	off_t		len = fileSize(filePath.c_str());
	if (len > 0)
	{
		Response	clientResponse(filePath, " 200 OK", contentType, getSockFd(), len);
		clientResponse.output();
		_response = clientResponse;
	}
}

void	Client::setPostResponse(string contentType) {
	off_t		len = 0;
	Response	clientResponse("201 Created", contentType, getSockFd(), len);
	_response = clientResponse;
}

bool	Client::responseSend() {
	cerr << "HasBody: " << _response.getHasBody() << endl;
	if (_response.getHasBody()) {
		cerr << "SendHeader: " << _response.getSendHeader() << endl;
		if (!_response.getSendHeader()) {
			_response.sendHeader();
			return true;
		}
		_response.sendBody();
		if (_response.getContentType() == "php")
		{
            if (remove(_response.getFilePath().c_str()) < 0)
                perror(_response.getFilePath().c_str());
		}
		return false;
	}
	if (!_response.getHasBody()) {
		_response.sendHeader();
	}
	return false;
}

void	Client::resetRequest() {
	Request	newRequest;

	this->_request = newRequest;
}

void Client::handleGetRequest(string filePath)
{
	string contentType;
	string extension;

//	filePath.append(file); // exception filePath;
	extension = filePath.substr(filePath.find_last_of('.') + 1);
	contentType = _contentType[extension];
	if (!contentType.empty())
		setResponse(filePath, contentType);
	else
		throw WSException::PageNotFound(); // not a supported extension
	return ;
}
//
//void Client::setPostContent(string input, int i) {
//	_postContent.push_back(vector<string>());
//
//}

void Client::handlePostRequest(string filepath)
{
	(void )filepath;
	string type = _request.getHeaderValue("Content-Type");

	if (type.compare("text/plain") == 0)
		parsePostPlainRequest();
	else if (type.compare("application/x-www-form-urlencoded") == 0)
		parsePostWwwRequest();
	else if (type.compare("multipart/form-data") == 0)
		parsePostMultipartRequest();
	setPostResponse(type);
}

void Client::handleDeleteRequest(string filePath, char** envp) {
	if (!fileAccess(filePath))
	{
		throw WSException::BadRequest();
	}
	Response	clientResponse(filePath.insert(0, "rm -f "), "200 OK", "php", getSockFd(), 0);
	clientResponse.setFilePath(clientResponse.CGIResponse(envp));
	setDeleteHTMLResponse(clientResponse.getFilePath());
	clientResponse.setFileSize(fileSize(clientResponse.getFilePath().c_str()));
	clientResponse.setNewHeader(" 200 OK", "php");
	clientResponse.output();
	_response = clientResponse;
}
