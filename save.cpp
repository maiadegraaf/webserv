#include "Request.hpp"

// request.cpp
Request::Request()
        : _method(""), _dir(""), _protocol(""), _file(""), _buffer(""), _body(""),
		_requestHeader(false), _requestBody(false) {}

Request::Request( const Request& rhs ) {
    *this = rhs;
}

Request::~Request() {}

Request&	Request::operator=( const Request& rhs ) {
    this->_dir = rhs._dir;
    this->_method = rhs._method;
    this->_protocol = rhs._protocol;
    this->_header = rhs._header;
    this->_input = rhs._input;
	this->_file = rhs._file;
	this->_buffer = rhs._buffer;

	return *this;
}
//
//Request::Request(string input) {
//    setInput(input);
//    setAttributes();
//    setContent();
//}

// Setters
void	Request::setInput(string input) {
    string vecPush;
    stringstream ss(input);

    while (getline(ss, vecPush, '\n')) {
        _input.push_back(vecPush);
    }
}

bool	Request::appendBuffer(string recvBuffer) {
	_buffer.append(recvBuffer);

	if (this->getRequestHeader() == true) {
		this->parseHeader();
		return true;
	}
	else if (this->getRequestBody() == true) {
		this->parseBody();
		return true;
	}
	return false;
}

void	Request::parseHeader() {
	stringstream	ss(_buffer);
	string			tmp;

	while (getline(ss, tmp)) {
		if (tmp.find("\r") == string::npos) {
			_buffer = tmp;
			return ;
		}
		else if (!tmp.compare("\r")) {
			this->setHeader();
			return ;
		}
		_input.push_back(tmp);
	}
}
void	Request::setHeader() {
	this->setAttributes();
	this->setContent();
	if ((_content.find("Content-Length") != _content.end() && \
			getContentValue("Content-Length").compare("0")) && !getMethod().compare("POST")) {
		this->setRequestBody(true);
	}
	this->setRequestHeader(false);
	_input.clear();
	_buffer.clear();
}

void	Request::parseBody() {
	stringstream	ss(_buffer);
	string			tmp;

	while (getline(ss, buffer)) {
		if (tmp.find("\r") == string::npos) {
			_buffer = tmp;
			return ;
		}
		else if (tmp.compare("\r")) {
			this->setRequestBody(false);
			return ;
		}
		_body.append(tmp);
		_body.append("\n");
	}
}

void	Request::setAttributes() {
    stringstream 	ss(_input[0]);
    string 			tmp;

    getline(ss, tmp, ' ');
    if (!tmp.empty() && !tmp.compare("GET") || !tmp.compare("POST") || \
	!tmp.compare("DELETE")) {
        throw WSException::MethodNotAllowed();
    }
    setMethod(tmp);
    getline(ss, tmp, ' ');
    if (!tmp.empty())
        throw WSException::BadRequest();
    setDir(tmp);
    getline(ss, tmp);
    if (!tmp.empty())
        throw WSException::BadRequest();
	if (!tmp.compare("HTTP/1.1")) {
		cerr << " http version "  << tmp << endl;
		throw WSException::HTTPVersionNotAvailable();
	}
    setProtocol(tmp);
}

void	Request::reset() {
	Request	newRequest;

	this = newRequest;
}

//
//void Request::setContent() {
//    string	key;
//    string	value;
//    size_t	idx;
//    size_t	i = 1;
//    size_t	size = _input.size();
//
//    while (i < size) { // && _input[i].compare(0, 1,"\n")) {
//        idx = _input[i].find(':', 0);
//        if (idx == string::npos)
//            break ;
//        key = _input[i].substr(0, idx);
//        value = _input[i].substr(idx + 2);
//        setContentValue(key, value);
//        i++;
//    }
//    _file.clear();
//    for (size_t j = i; j < size; j++) {
//        _file.append(_input[j]);
//        _file.append("\n");
//    }
//}

// Output
//void Request::output() {
//    std::cout << "_method : " << _method << std::endl;
//    std::cout << "_dir : " << _dir << std::endl;
//    std::cout << "_protocol : " << _protocol << std::endl;
//}

Request.hpp
#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "WSException.hpp"

using namespace std;

class Request
{
	/* *******************
	 * (Con/De)structors *
	 * *******************/
	public:
		Request();
		Request( const Request &rhs );
		~Request();
		Request& operator=( const Request &rhs );
//		Request(string input);

	/* ************
	 * Attributes *
	 * ************/
	private:
		string				_method;
		string				_dir;
		string				_protocol;
		vector<string>		_input;
		map<string, string> _header; // maybe call header
		string 				_buffer,
							_body;
		bool 				_requestHeader,
							_requestBody;
//							_setWrite;

	/* *********
	 * Getters *
	 * *********/
	public:
		string			getMethod()							{ return _method; }
		string			getDir() 							{ return _dir; }
		string			getProtocol()						{ return _protocol; }
		string			getHeaderValue(string key)			{ return _header[key]; }
		vector<string>	getInput()							{ return _input; }
		string 			getBody()							{ return _body; }

	/* *********
	 * Setters *
	 * *********/
	public:
		void	setMethod(string new_method)				{ this->_method = new_method; }
		void	setDir(string new_dir)						{ this->_dir = new_dir; }
		void	setProtocol(string new_protocol)			{ this->_protocol = new_protocol; }
		void	setHeaderValue(string key, string value)	{ this->_header[key] = value; }
		void 	setRequestHeader(bool newBool)				{ this->_requestHeader = newBool; }
		void 	setRequestBody(bool newBool)				{ this->_requestBody = newBool; }

		void 	parseHeader();
		void 	setHeader();
		void 	parseBody();
		void	setAttributes();
		void	setContent();

	/* ********
	 * Output *
	 * ********/
        void	output();

};
 
#endif

Response.cpp
#include "Response.hpp"
#include "Server.hpp"

Response::Response(string errorMessage, int newSockFD, string contentType)
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath("www/error/") {
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

Response::Response(string filePath, string message, string contentType, int newSockFD, off_t fileSize) // keep alive must be there
	: _sockFD(newSockFD), _head("HTTP/1.1 "), _filePath(filePath), _fileSize(fileSize) {
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
	return *this;
}
 
// Output
void Response::output() {
	std::cout << "sockFD : " << _sockFD << std::endl;
	cout << "head : " << _head << endl;
	std::cout << "fileSize : " << _fileSize << std::endl;
}

bool Response::sendResponse() {
	int read = open(getFilePath().c_str(), O_RDONLY);
	send(getSockFD(), _head.c_str(), _head.size(), 0);
	if (sendfile(read, getSockFD(), 0, &_fileSize, NULL, 0) < 0) {
		cerr << "send() failed " << errno << endl;
		return close(read), false;
	}
	close(read);
	return true;
}

Response.hpp
#ifndef RESPONSE_H
#define RESPONSE_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sys/socket.h>
#include <sstream>
#include <map>
#include <fcntl.h>
#include "webserv.h"
#include "Utils.h"
#include "WSException.hpp"


using namespace std;

class Response
{
	/* ********************
	 * (Con/De)structor's *
	 * ********************/
	public:
		// Canonical Form
		Response()
			: _sockFD(0), _head(""), _filePath(""), _fileSize(0)					{ }
		~Response()																		{ }
		Response(const Response &rhs) 													{ *this = rhs; }
		Response&	operator=( const Response &rhs);

		// Error & Regular construction
        Response(string errorMessage, int newSockFD, string contentType);
        Response(string filePath, string message, string contentType, \
		int newSockFD, off_t fileSize);

	/* ************
	 * Attributes *
	 * ************/
	private:
		int			_sockFD;
		string		_head;
		string		_filePath;
		off_t 		_fileSize;

	/* *********
	 * Getters *
	 * *********/
	public:
		int		getSockFD()								{ return _sockFD; }
		string	getHead()								{ return _head; }
		string	getFilePath()							{ return _filePath; }
		off_t 	getFileSize()							{ return _fileSize; }

	/* *********
	 * Setters *
	 * *********/
	public:
		void	setSockFD(int newSockFD)						{ this->_sockFD = newSockFD; }
		void	setHead(string newHead)							{ this->_head = newHead; }
		void	setFilePath(string newFilePath)					{ this->_filePath = newFilePath; }
		void	setFileSize(off_t newFileSize)					{ this->_fileSize = newFileSize; }
		void	appendToHead(string newHead)					{ this->_head.append(newHead); }
		void 	appendToHeadNL(string newHead)					{ appendToHead(newHead); this->_head.append("\r\n"); }
		void 	appendObjectToHead(string type, string name)	{ appendToHead(type); appendToHeadNL(name); }
		void	appendToFilePath(string newPath)				{ this->_filePath.append(newPath); }

	/* ********
	 * Output *
	 * ********/
	public:
		void	output();
		bool	sendResponse();
//		bool 	sendCGI();
};
 
#endif

ServerKq.cpp
#include "Server.hpp"

void	Server::setupKq(int kq) {
	_kq = kq;
	EV_SET(&_changeEvent[0], getSockFd(), EVFILT_READ, EV_ADD, 0, 0, 0);
	EV_SET(&_changeEvent[1], getSockFd(), EVFILT_WRITE, EV_ADD, 0, 0, 0);
	if (kevent(getKq(), _changeEvent, 2, NULL, 0, NULL) == -1) {
		perror("kevent server read error");
		exit(1);
	}
	this->setKqRead();
}

void	Server::enableKqRead() {
	EV_SET(&_changeEvent[0], getSockFd(), EVFILT_READ, EV_ENABLE, 0, 0, 0);
	EV_SET(&_changeEvent[1], getSockFd(), EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
	if (kevent(getKq(), _changeEvent, 2, NULL, 0, NULL) == -1) {
		perror("kevent server read error");
		exit(1);
	}
}

void	Server::enableKqWrite() {
	EV_SET(&_changeEvent[0], getSockFd(), EVFILT_READ, EV_DISABLE, 0, 0, 0);
	EV_SET(&_changeEvent[1], getSockFd(), EVFILT_WRITE, EV_ENABLE, 0, 0, 0);
	if (kevent(getKq(), _changeEvent, 2, NULL, 0, NULL) == -1) {
		perror("kevent server write error");
		exit(1);
	}
}

void	Server::disableKq() {
	EV_SET(&_changeEvent[0], getSockFd(), EVFILT_READ, EV_DISABLE, 0, 0, 0);
	EV_SET(&_changeEvent[1], getSockFd(), EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
	if (kevent(getKq(), _changeEvent, 2, NULL, 0, NULL) == -1) {
		perror("kevent disable server error");
		exit(1);
	}
}

void	Server::clientNewAcceptFd(int eventFd) {
	int	opt_value = 1;

	_acceptFd = accept(eventFd, (struct sockaddr *) &_client_addr, (socklen_t * ) & _len);
	if (_acceptFd == -1)
		perror("Accept socket error");
	setsockopt(eventFd, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));
	setsockopt(eventFd, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));
	if (fcntl(_acceptFd, F_SETFL, O_NONBLOCK) < 0) {
		cerr << "fcntl failed: to make the acceptFd unblocking" << endl;
		close(_acceptFd);
		exit(-1);
	}
}

void	Server::bindServerAcceptFdWithClient() {
	struct kevent	newEvents[2];
	Client *newClient = new Client(getAcceptFd(), getLocation(), getContentType(), getMaxSize());
	EV_SET(&newEvents[0], getAcceptFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, newClient);
	EV_SET(&newEvents[1], getAcceptFd(), EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, newClient);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0) {
		perror("kevent error");
	}
}

string	Server::readFile(string FilePath) {
	char 	*buf;
	off_t 	fSize;

	_readFd = open(filePath.c_str(), O_RDONLY);
	fSize = lseek(_readFd, 0, SEEK_END);
	buf = new [fSize] char;
	read(_readFd, buf, (size_t)fSize);
	close(_readFd);
	string	readB(buf);
	delete buf;
	return readB;
}

Server.cpp
#include "Server.hpp"

Server::Server(Config *conf)
		: _sockFd(-1), _readFd(-1), _acceptFd(-1), _conf(conf), _closeConnection(false) {
	this->setup();
}

Server&	Server::operator=( const Server& rhs ) {
	this->_sockFd = rhs._sockFd;
	this->_kq = rhs._kq;
	this->_len = rhs._len;
	this->_acceptFd = rhs._acceptFd;
	this->_servAddr = rhs._servAddr;
	this->_client_addr = rhs._client_addr;
	this->_conf = rhs._conf;
	this->_changeEvent[0] = rhs._changeEvent[0];
	this->_changeEvent[1] = rhs._changeEvent[1];
	this->_contentType = rhs._contentType;
	this->_location = rhs._location;
	this->_closeConnection = rhs._closeConnection;
	this->_maxSize = rhs._maxSize;

	return *this;
}

void Server::setup() {
	setMaxSize(static_cast<size_t>(_conf->getMaxSize()));
	_contentType = returnContentType();
	_location = _conf->getLocation();
	this->setupSockFd();
	this->setupSocketOpt();
	this->setupNonBlock();
	this->setAddr();
	this->bindSocket();
	this->listenSocket();
	_len = sizeof(_client_addr);
}

void	Server::setupSockFd() {
	_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd < 0) {
		cerr << "could not create socket (server)" << endl;
		exit(-1);
	}
}

void	Server::setupSocketOpt() {
	int on = 1;

	if (setsockopt(getSockFd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
		cerr << "setsockopt() failed" << endl;
		close(getSockFd());
		exit(-1);
	}
}

void	Server::setupNonBlock() {
	if (fcntl(getSockFd(), F_SETFL, O_NONBLOCK) < 0) {
		cerr << "fcntl failed: to make the socket unblocking" << endl;
		close(getSockFd());
		exit(-1);
	}
}

void Server::setAddr() {
	bzero((char*)&_servAddr, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr.sin_port = htons(_conf->getAddress());
}

void	Server::bindSocket() {
	if (bind(getSockFd(), (struct sockaddr*) &_servAddr, sizeof(_servAddr)) < 0) {
		cerr << "Error binding socket to local address" << endl;
		close(getSockFd());
		exit(-1);
	}
}

void	Server::listenSocket() {
	if (listen(getSockFd(), 32) < 0) {
		cerr << "Listen failed" << endl;
		close(getSockFd());
		exit(-1);
	}
}

void	Server::output() {
	cout << "acceptFd: " << getAcceptFd() << endl;
	cout << "kq: " << getKq() << endl;
	cout << "fd or sockfd: " << getSockFd() << endl;
}

Webserv.cpp
#include "ServerIO.hpp"
// Constructor initializes attributes to 0 by default 

ServerIO::ServerIO( const ServerIO& rhs) {
	*this = rhs;
}

ServerIO&	ServerIO::operator=( const ServerIO& rhs ) {
	(void)rhs;
	return *this;
}

ServerIO::ServerIO(vector<Config> newConfig, map<string, string> newContentType)
	:_config(newConfig), _contentType(newContentType) {
	size_t	size = _config.size();

	for (size_t i = 0; i < size; i++) {
		Server	tmp(&_config[i]);
		_server.push_back(tmp);
	}
	_serverSize = size;
	runServerIO();
}

void	ServerIO::runServerIO() {
	initKq();
	while(1) {
		newEvent();
		loopEvent();
	}
}

void	ServerIO::initKq() {
	cout << "initializing kqueue..." << endl;
	_kq = kqueue();
	for (size_t idx = 0; idx < getServerSize(); idx++) {
		_server[idx].setupKq(getKq());
		_sockFdIdxMap[_server[idx].getSockFd()] = idx;
	}
}

void	ServerIO::newEvent() {
	_nrEvents = kevent(getKq(), NULL, 0, _events, 2, NULL);
	if (_nrEvents == -1) {
		perror("kevent");
		exit(1);
	}
}

void	ServerIO::loopEvent( ) {
	struct kevent	event;

	for (int i = 0; i < _nrEvents; i++) {
		event = _events[i];
		_eventFd = event.ident;
		if (event.flags == EV_EOF)
			this->disconnectClient(event.udata);
		else if (_sockFdIdxMap.find(_eventFd) != _sockFdIdxMap.end())
			this->connectNewClient();
		else if (event.filter == EVFILT_READ) // event
			this->readOperation(event.udata);
		else if (event.filter == EVFILT_WRITE)
			this->writeOperation(event.ident);
	}
}

void	ServerIO::disconnectClient(void *udata) {
	printf("Client has disconnected\n");
	close(_eventFd);
	Client *client = static_cast<Client *>(udata);
	delete client;
}

void	ServerIO::connectNewClient() {
	size_t			idx;

	printf("New connection coming in...\n");
	idx = _sockFdIdxMap[_eventFd];
	_server[idx].clientNewAcceptFd(_eventFd);
	_server[idx].bindServerAcceptFdWithClient();
}

void	ServerIO::readOperation(void *udata) {
	size_t	idx = _sockFdIxMap[_eventFd]; // is idx correct for the opperation
	Client	*client = reinterpret_cast<Client *>(udata);
	if (client) {
		cerr << "doing the request\n";
		cerr << "eventFd :" << _eventFd << endl;
		if (client->clientInRequest() == false) { // this is only for read
			_server[idx].setResponse(&client->getResponse()); // more elaborate on Get Post Delete
			_server[idx].setKqRead();
			client->resetRequest();
		}
	}
	else {
		string	buf =	_server[idx].readFile();
		_server[idx]. // do somethign with buf;
	}
}

//void	ServerIO::writeOperation()

// Output
void ServerIO::output() {
	std::cout << "kq : " << _kq << std::endl;
}

Client.cpp
#include "Client.hpp"
// Constructor initializes attributes to 0 by default


Client::Client(int newSockFD, map<string, Location> newLocation, map<string, string> newContentType, \
size_t newMaxSize)
	: _sockFD(newSockFD), _len(-1), _contentType(newContentType), _location(newLocation), \
	_requestBuffer(""), _maxSize(newMaxSize), _closeConnection(false) { }


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

bool	Client::clientInRequest() {
	try {
		this->fillRequestBuffer();
		if (_request.appendBuffer(this->getRequestBuffer()) == false) {
			this->setResponse();
			return true;
		}
		return false;
	} catch (exception &e) {
		string		tmpMessage(e.what());
		Response	error(tmpMessage, _sockFD, getContentType("html"));
		return true;
	}
}

void	Client::fillRequestBuffer() {
	int     rc;
	char    buffer[200];

	rc = recv(_sockFD, buffer, sizeof(buffer), 0);
	if (recvError(rc))
		exit(-1) ;
	_requestBuffer.append(tmp);
}

//void	Client::addClientRequest() {
//	re
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
//		tmp.assign(buffer, rc);
//		request.append(tmp);
//	}
//	if (request.size() > getMaxSize())
//		throw WSException::PayloadTooLarge();
//	if (request.size() == 0)
//		throw WSException::BadRequest();
//	return request;
//}

bool	Client::recvError(int rc) {
	if (rc < 0) {
		cerr << errno << ": errno\n";
		perror("recv()");
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

void	Client::setupResponse() { // should we use a --> const Request &ref ??
	string		filePath("www/");
	string 		confFile;
	Location	loca;
	string		file;
	string		extension;
	string		contentType;

	cerr << "clientReq " << _request.getDir() << endl;
	loca = getLocation(_request.getDir());
	confFile = loca.getIndex();
	if (!confFile.empty())
		file.append(confFile); // page not foudn exception
	else {
		if (clientReq.getDir().empty())
			throw WSException::BadRequest();
		file.append(_request.getDir()); // Response
	}
	filePath.append(file); // exception filePath;
	extension = filePath.substr(filePath.find_last_of('.') + 1);
//	if (extension.compare("php") == 0) {
//		handleCGIResponse(filePath, _contentType["html"]);
//		return ;
//	}
	contentType = _contentType[extension];
	if (!contentType.empty())
		setResponse(filePath, contentType);
	else
		throw WSException::PageNotFound(); // not a supported extension
	return ;
}

void	Client::setResponse(string filePath, string contentType) {
	off_t		len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, getSockFd(), len);

	_response = clientResponse;
//	if (getCloseConnection() == true)
//		send(getSockFD(), 0, 1, 0);
//	if (!clientResponse.sendResponse())
//		setCloseConnection(true);
//	_strRequest.clear();
}

void	Client::handleCGIResponse(string filePath, string contentType) {
	off_t _len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, _sockFD, _len); // different lenght constructor for cgi
	if (!clientResponse.sendResponse())
		setCloseConnection(true);
	_strRequest.clear();
}

Client.hpp
#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <string>
#include <map>
#include "Response.hpp"
#include "Request.hpp"
#include "WSException.hpp"
#include "Location.hpp"


// Class definition
class Client {
	/* *******************
	 * (Con/De)structors *
 	* *******************/
	public:
		// Constructor
		~Client()																				{}
		Client( const Client &rhs)																{ *this = rhs; }
		Client& operator=( const Client &rhs);
		Client()
			: _sockFD(-1), _strRequest(""), _maxSize(0) 										{}
		Client(int newSockFD, map<string, Location> newLocation, \
			map<string, string> newContentType, size_t newMaxSize);


	/* ************
	 * Attributes *
	 * ************/
	private:
		int 								_sockFD,
											_len,
											_read;
		map<string, string>					_contentType;
		map<string, Location>		        _location;
		string								_requestBuffer;
		size_t								_maxSize;
		bool 								_closeConnection;
		Request								_clientRequest;
		Response							_response;

		// this can be probably removed
		// Add in the 5 string variables here from handleRequest() and put in getters/setters as well for them

	/* *********
 	* Getters *
 	* *********/
	public:
		int					getSockFD() const													{ return this->_sockFD; }
		const string		&getContentType(string key)											{ return this->_contentType[key]; }
		Location			&getLocation(string key)											{ return this->_location[key]; }
		string				getStrRequest() const												{ return this->_strRequest; }
		bool 				getCloseConnection() const											{ return this->_closeConnection; } // this can probably be removed
		size_t 				getMaxSize() const													{ return this->_maxSize; }

	/* *********
 	* Setters *
 	* *********/
	public:
		void		setSockFD(int newSockFD)													{ this->_sockFD = newSockFD; }
		void		setStrRequest(string newRequest)											{ this->_strRequest = newRequest; }
		void		setCloseConnection(bool Bool)												{ this->_closeConnection = Bool; } // this can probably be removed

	/* **************
 	* Functionality *
 	* ***************/
	public:
		void		output();
		void 		clientRequest();
		string		receiveStrRequest();
		bool 		recvError(int rc);
		void 		handleRequest(Request clientReq);
		void		handleResponse(string filePath, string contentType);
		void		handleCGIResponse(string filePath, string contentType);

};

#endif

