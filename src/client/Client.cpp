#include "Client.hpp"
// Constructor initializes attributes to 0 by default


Client::Client(int newSockFd, map<string, Location> newLocation, map<string, string> newContentType, \
size_t newMaxSize)
	: _sockFd(newSockFd), _len(-1), _contentType(newContentType), _location(newLocation), _requestBuffer(""), \
	_maxSize(newMaxSize), _requestMode(true) {
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
	//cout << "SockFD : " << _sockFd << std::endl;
	//cout << "strRequest : " << _requestBuffer << std::endl;
}

bool	Client::requestReceived() {
	cerr << _request.getMethod() << endl;
	try {
		this->fillRequestBuffer();
//		if (this->getRequestMode() == false) // check hier
//			return false;
		if (_request.appendBuffer(getRequestBuffer()) == false) {
			cerr << "please arrive here\n";
			this->handleRequest();
			this->resetRequest();
			return true;
		}
		return false;
	} catch (exception &e) {
		string		tmpMessage(e.what());
		Response	error(tmpMessage, getSockFd(), getContentType("html"));
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
		//cerr << "recv() sockfFd :" << getSockFd() << ": stopped reading " << endl;
		perror("recv error");
		exit(-1);
//		return true ;
	}
	if (rc == 0) {
		//cerr << "everything read in the client" << endl;
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
//		//cerr << "recv() stopped reading " << endl;
//		perror("recv error");
//		return true ;
//	}
//	if (rc == 0) {
//		//cerr << "everything read in the client" << endl;
////		this->setRequestMode(false); // check hier
//		return true ;
//	}
//	return false ;
//}

void	Client::handleRequest() {
	string		filePath("www/");
	string 		confFile;
	Location	loca;
	string		file;
	string		extension;
	string		contentType;

	_request.output();

//	//cerr << "clientReq " << _request.getDir() << endl;
	cout << _request.getMethod() << endl;
	loca = getLocation(_request.getDir());
	confFile = loca.getIndex();
	if (!confFile.empty())
		file.append(confFile); // page not foudn exception
	else {
		if (_request.getDir().empty())
			throw WSException::BadRequest();
		file.append(_request.getDir()); // Response
	}
	if (_request.getMethod().compare("GET") == 0) {
		handleGetRequest(file, filePath);
		cout << "this is a GETTT" << endl;
	}
	else if (_request.getMethod().compare("POST") == 0) {
		handlePostRequest(file, filePath);
		cout << "this is a POSTTT" << endl;
	}
	cout << "get Mthod: -> " << _request.getMethod() << endl << endl;
//		if (extension.compare("php") == 0) {
//		handleCGIResponse(filePath, _contentType["html"]);
//		return ;
//	}

}

void	Client::setResponse(string filePath, string contentType) {
	off_t		len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, getSockFd(), len);
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
		return false;
	}
	_response.sendHeader();
	return false;
}

void	Client::resetRequest() {
	Request	newRequest;

	this->_request = newRequest;
}

void Client::handleGetRequest(string file, string filePath)
{
	string contentType;
	string extension;

	filePath.append(file); // exception filePath;
	extension = filePath.substr(filePath.find_last_of('.') + 1);
	contentType = _contentType[extension];
	cerr << "contentType: " << contentType << endl;
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

void Client::handlePostRequest(string file, string filepath)
{
	(void )filepath;
	(void )file;
	string type = _request.getHeaderValue("Content-Type");

	cerr << type << endl;
	if (type.compare("text/plain") == 0)
	{
		parsePostPlainRequest();
		cerr << "sdfdsfdfsfdsfds" << endl;
	}
	else if (type.compare("application/x-www-form-urlencoded") == 0)
		parsePostWwwRequest();
	else if (type.compare("multipart/form-data") == 0)
		parsePostMultipartRequest();
	setPostResponse(type);
}



//void	Client::clientRequest() {
//	try {
//		Request	clientReq(receiveStrRequest());
//		handleRequest(clientReq);
//	} catch (exception &e) {
//		string		tmpMessage(e.what());
//		Response	error(tmpMessage, _sockFD, getContentType("html"));
//		error.sendResponse();
//	}
//}
//
//string	Client::receiveStrRequest() {
//	int     rc;
//	char    buffer[100];
//	string	request("");
//	string	tmp;
//
//	while (1) {
//		rc = recv(_sockFD, buffer, sizeof(buffer), 0);
//		if (recvError(rc))
//			break ;
////		_len = rc;
//		tmp.assign(buffer, rc);
//		request.append(tmp);
//	}
//	if (request.size() > getMaxSize())
//		throw WSException::PayloadTooLarge();
//	if (request.size() == 0)
//		throw WSException::BadRequest();
//	return request;
//}

//void	Client::handleRequest(Request clientReq) { // should we use a --> const Request &ref ??
//	string filePath("www/");
//	string confFile;
//	Location loca;
//	string file;
//	string extension;
//	string contentType;
//
//	//cerr << "clientReq " << clientReq.getDir() << endl;
//	loca = getLocation(clientReq.getDir());
//	confFile = loca.getIndex();
//	if (!confFile.empty())
//		file.append(confFile); // page not foudn exception
//	else {
//		if (clientReq.getDir().empty())
//			throw WSException::BadRequest();
//		file.append(clientReq.getDir()); // Response
//	}
//	filePath.append(file); // exception filePath;
//	extension = filePath.substr(filePath.find_last_of('.') + 1);
//	if (extension.compare("php") == 0) {
//		handleCGIResponse(filePath, _contentType["html"]);
////		if (getCloseConnection() == true)
////			return ;
//		return ;
//	}
//	contentType = _contentType[extension];
//	if (!contentType.empty()) {
//		handleResponse(filePath, contentType);
////		if (getCloseConnection() == true)
////			return ;
//	} else
//		throw WSException::PageNotFound(); // not a supported extension
//	return ;
//}
//
//void	Client::handleResponse(string filePath, string contentType) {
//	off_t _len = fileSize(filePath.c_str());
//	Response	clientResponse(filePath, "200 OK", contentType, _sockFD, _len);
//	if (!clientResponse.sendResponse())
//		setCloseConnection(true);
//	_strRequest.clear();
//}
//
//void	Client::handleCGIResponse(string filePath, string contentType) {
//	off_t _len = fileSize(filePath.c_str());
//	Response	clientResponse(filePath, "200 OK", contentType, _sockFD, _len); // different lenght constructor for cgi
//	if (!clientResponse.sendResponse())
//		setCloseConnection(true);
//	_strRequest.clear();
//}
