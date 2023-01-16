#include "Client.hpp"

Client::Client(int newSockFd, map<string, Location> newLocation, string newRoot, map<int, string> newErrorPages,  map<string, string> newContentType, \
size_t newMaxSize)
	: _sockFd(newSockFd), _len(-1), _contentType(newContentType), _location(newLocation), \
	_requestBuffer(""), _root(newRoot), _errorPages(newErrorPages), _maxSize(newMaxSize), _requestMode(true) {
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

bool	Client::requestReceived() {
	try {
		this->fillRequestBuffer();
//		if (this->getRequestMode() == false) // check hier
//			return false;
		if (_request.appendBuffer(getRequestBuffer()) == false) {
			cerr << "please arive here\n";
			this->handleRequest();
			this->resetRequest();
			return true;
		}
		return false;
	} catch (exception &e) {
		string		tmpMessage(e.what());
		string		filePath("default/");
		int 		errorNr = atoi(tmpMessage.c_str());
		filePath.append(getErrorPageValue(errorNr));
		cout << "this is Filepath :" << filePath << endl;
		Response	error(tmpMessage, filePath, getSockFd(), getContentType("html"));
		_response = error;
		return true;
	}
}

void	Client::fillRequestBuffer() {
	int     rc;
	char    buffer[200];
	string	tmp;

	rc = recv(getSockFd(), buffer, sizeof(buffer), 0);
	if (rc < 0) {
		cerr << "recv() sockfFd :" << getSockFd() << ": stopped reading " << endl;
		perror("recv error");
		exit(-1);
//		return true ;
	}
	if (rc == 0) {
		cerr << "everything read in the client" << endl;
		return ;
//		this->setRequestMode(false); // check hier
//		return true ;
	}
//	if (recvError(rc)) {
//		return ;
////		exit(-1);
//	}
	tmp.assign(buffer, rc);
	_requestBuffer = tmp;
}

//bool	Client::recvError(int rc) {
//	if (rc < 0) {
//		cerr << "recv() stopped reading " << endl;
//		perror("recv error");
//		return true ;
//	}
//	if (rc == 0) {
//		cerr << "everything read in the client" << endl;
////		this->setRequestMode(false); // check hier
//		return true ;
//	}
//	return false ;
//}

void	Client::handleRequest() {
	Location	location;
	string		filePath(getRoot().append("/"));
	string 		path;
//	string		file;
//	string		extension;
//	string		contentType;

	_request.output();

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
		if (extension(filePath) == "php") {
			handleCGIResponse(filePath, "php");
			return ;
		}
		handleGetRequest(filePath);
	}
	else if (_request.getMethod() == "POST") {
		handlePostRequest(filePath, _request);
	}
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
	Response	clientResponse(filePath, "200 OK", contentType, getSockFd(), len);
	clientResponse.output();
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
        {
			cerr << "HELLO" << endl;
            remove(_response.getFilePath().c_str());
        }
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

void Client::handlePostRequest(string filepath, Request clientReq)
{
//	string line;
//	string len;
//	string disp;
//	string cont_type;
//	string filename;
//	string savePath;
	(void )filepath;
//	(void )file;
	string type = clientReq.getHeaderValue("Content-Type");

	if (type.compare("text/plain") == 0)
		parsePostPlainRequest(clientReq);
	else if (type.compare("application/x-www-form-urlencoded") == 0)
		parsePostWwwRequest(clientReq);
	else if (type.compare("multipart/form-data") == 0)
		parsePostMultipartRequest(clientReq);
}

//
//void	Client::handleResponse(string filePath, string contentType) {
//	off_t _len = fileSize(filePath.c_str());
//	Response	clientResponse(filePath, "200 OK", contentType, _sockFD, _len);
//	if (!clientResponse.sendResponse())
//		setCloseConnection(true);
//	_strRequest.clear();
//}
//
