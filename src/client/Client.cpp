#include "Client.hpp"
// Constructor initializes attributes to 0 by default


Client::Client(int newSockFD, map<string, Location> newLocation, map<string, string> newContentType, \
size_t newMaxSize)
	: _sockFD(newSockFD), _len(-1), _contentType(newContentType), _location(newLocation), _strRequest(""), \
	_maxSize(newMaxSize), _closeConnection(false) {
//	cerr << "this is sockfd:" << _sockFD << ":\n";
}

Client&	Client::operator=( const Client& rhs ) {
	this->_sockFD = rhs._sockFD;
	this->_len = rhs._len;
	this->_location = rhs._location;
	this->_contentType = rhs._contentType;
	this->_strRequest = rhs._strRequest;
	this->_maxSize = rhs._maxSize;
	this->_closeConnection = rhs._closeConnection;
	return *this;
}

// Output
void	Client::output() {
	std::cout << "SockFD : " << _sockFD << std::endl;
//	std::cout << "fds : " << _fds << std::endl;
//	std::cout << "location : " << _location << std::endl;
	std::cout << "strRequest : " << _strRequest << std::endl;
}

void	Client::clientRequest() {
	try {
		Request	clientReq(receiveStrRequest());
		handleRequest(clientReq);
	} catch (exception &e) {
		string		tmpMessage(e.what());
		Response	error(tmpMessage, _sockFD, getContentType("html"));
		error.sendResponse();
	}
}

string	Client::receiveStrRequest() {
	int     rc;
	char    buffer[100];
	string	request("");
	string	tmp;

	while (1) {
		rc = recv(_sockFD, buffer, sizeof(buffer), 0);
		if (recvError(rc))
			break ;
//		_len = rc;
		tmp.assign(buffer, rc);
		request.append(tmp);
	}
	if (request.size() > getMaxSize())
		throw WSException::PayloadTooLarge();
	if (request.size() == 0)
		throw WSException::BadRequest();
	return request;
}

bool	Client::recvError(int rc) {
	if (rc < 0) {
		cerr << "recv() stopped reading " << endl;
		this->setCloseConnection(true);
		return true ;
	}
	if (rc == 0) {
		cerr << "  Connection closed" << endl;
		this->setCloseConnection(true);
		return true ;
	}
	return false ;
}

void	Client::handleRequest(Request clientReq) { // should we use a --> const Request &ref ??
	string filePath("www/");
	string confFile;
	Location loca;
	string file;
	string extension;
	string contentType;

	cerr << "clientReq " << clientReq.getDir() << endl;
	loca = getLocation(clientReq.getDir());
	confFile = loca.getIndex();
	if (!confFile.empty())
		file.append(confFile); // page not foudn exception
	else {
		if (clientReq.getDir().empty())
			throw WSException::BadRequest();
		file.append(clientReq.getDir()); // Response
	}
	filePath.append(file); // exception filePath;
	extension = filePath.substr(filePath.find_last_of('.') + 1);
	if (extension.compare("php") == 0) {
		handleCGIResponse(filePath, _contentType["html"]);
//		if (getCloseConnection() == true)
//			return ;
		return ;
	}
	contentType = _contentType[extension];
	if (!contentType.empty()) {
		handleResponse(filePath, contentType);
//		if (getCloseConnection() == true)
//			return ;
	} else
		throw WSException::PageNotFound(); // not a supported extension
	return ;
}

void	Client::handleResponse(string filePath, string contentType) {
	off_t _len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, _sockFD, _len);
	if (!clientResponse.sendResponse())
		setCloseConnection(true);
	_strRequest.clear();
}

void	Client::handleCGIResponse(string filePath, string contentType) {
	off_t _len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, _sockFD, _len); // different lenght constructor for cgi
	if (!clientResponse.sendResponse())
		setCloseConnection(true);
	_strRequest.clear();
}
