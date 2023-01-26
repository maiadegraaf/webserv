#include "Client.hpp"

Client::Client(int newSockFd, map<string, Location> newLocation, string newRoot, map<int, string> newErrorPages,  map<string, string> newContentType, \
size_t newMaxSize)
	: _sockFd(newSockFd), _len(-1), _contentType(newContentType), _location(newLocation), \
	_requestBuffer(""), _root(newRoot), _errorPages(newErrorPages), _maxSize(newMaxSize), _clientMode(request), _endOfRequest(false), _isParsing(false) {
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
	this->_clientMode = rhs._clientMode;
	return *this;
}

// Output
void	Client::output() {
	std::cout << "SockFD : " << _sockFd << std::endl;
	std::cout << "strRequest : " << _requestBuffer << std::endl;
}

bool 	Client::requestReceived() {
	try {
		this->fillRequestBuffer();

		stringstream ss(getRequestBuffer());
		_request.setSS(&ss);
		_request.parseBuffer();
		this->handleRequest();
//		cerr << "\033[1;31m" << _requestBuffer << "\033[0m" << endl;
		_requestBuffer.clear();
		return true;
	} catch (exception &e) {
		string		tmpMessage(e.what());
		string		filePath("default/");
		int 		errorNr = atoi(tmpMessage.c_str());
		filePath.append(getErrorPageValue(errorNr));
		cout << "this is Filepath :" << filePath << endl;
		Response	error(tmpMessage, filePath, getSockFd(), getContentType("html"));
		_response = error;
		this->setClientMode(response);
		return true;
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
//		cerr << "\033[1;36m" <<" ------------------- "<< "\033[0m" << endl;
//		cerr << "\033[1;32m" << _requestBuffer << "\033[0m" << endl;
	}
}

void	Client::handleRequest() {
	Location	location;
	string		filePath(getRoot().append("/"));
	string 		path;
//	string		file;
//	string		extension;
//	string		contentType;

//	_request.output();

//	cerr << "clientReq " << _request.getDir() << endl;
	location = getLocation(_request.getDir());
	path = location.getIndex();
	if (!path.empty())
		filePath.append(_request.getDir() + '/' + path); // page not foudn exception
	else {
		if (_request.getDir().empty())
			throw WSException::BadRequest();
		filePath.append(_request.getDir()); // Response
	}
	if (_request.getMethod() == "GET")
	{
		if ( filePath.find("php") != string::npos ) {
			handleCGIResponse(filePath, "php");
			this->resetRequest();
			return ;
		}
		handleGetRequest(filePath);
	}
	else if (_request.getMethod() == "POST") {
		handlePostRequest(filePath);
	}
	else if (_request.getMethod() == "DELETE") {
		handleDeleteRequest(filePath, _request);
	}
	this->resetRequest();
}

void	Client::handleCGIResponse(const string& filePath, const string& contentType) {
	Response	clientResponse(filePath, "200 OK", contentType, getSockFd(), 0);
	clientResponse.setFilePath(clientResponse.CGIResponse());
	clientResponse.setFileSize(fileSize(clientResponse.getFilePath().c_str()));
	clientResponse.setNewHeader("200 OK", contentType);
	_response = clientResponse;
}

void	Client::setResponse(string filePath, string contentType) {
	off_t		len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, " 200 OK", contentType, getSockFd(), len);
	clientResponse.output();
	_response = clientResponse;
}

void	Client::setPostResponse(string contentType) {
	off_t		len = 0;
	Response	clientResponse("201 Created", contentType, getSockFd(), len);
	_response = clientResponse;
}

bool	Client::responseSend() {
	if (_response.getHasBody() == true) {
		if (_response.getSendHeader() == false) {
			_response.sendHeader();
			return true;
		}
		_response.sendBody();
		if (_response.getContentType() == "php")
            remove(_response.getFilePath().c_str());
		return false;
	}
	_response.sendHeader();
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

void Client::handleDeleteRequest(string filePath, Request clientReq) {
	string type = clientReq.getHeaderValue("Content-Type");
	if (!fileAccess(filePath))
		throw WSException::BadRequest();
	Response	clientResponse(filePath.insert(0, "/bin/rm -f "), "200 OK", type, getSockFd(), 0);
	clientResponse.setFilePath(clientResponse.CGIResponse());
	setDeleteHTMLResponse(clientResponse.getFilePath());
	clientResponse.setFileSize(fileSize(clientResponse.getFilePath().c_str()));
	clientResponse.setNewHeader(" 200 OK", type);
	_response = clientResponse;
}
