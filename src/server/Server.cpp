#include "Server.hpp"

Server::Server(Config *conf)
		: _sockFd(-1), _acceptFd(-1), _conf(conf), _closeConnection(false) {
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
	this->setAddr();
	this->setupSocketOpt();
	this->setupNonBlock();
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
	memset(_servAddr.sin_zero, '\0', sizeof(_servAddr.sin_zero));
}

void	Server::bindSocket() {
	if (bind(getSockFd(), (struct sockaddr*) &_servAddr, (socklen_t)sizeof(_servAddr)) < 0) {
		cerr << "Error binding socket to local address" << endl;
		close(getSockFd());
		exit(-1);
	}
}

void	Server::listenSocket() {
	if (listen(getSockFd(), SOMAXCONN) < 0) {
		cerr << "Listen failed" << endl;
		close(getSockFd());
		exit(-1);
	}
}


//ListeningSocket::ListeningSocket( int port ) : _port(port), _enable(1) {
//
//	init_address();
//	_addrLen = sizeof(_address);
//	_fd = socket(AF_INET, SOCK_STREAM, 0);
//	if (_fd == ERROR)
//		throw(error(SOCKET, port));
//	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &_enable, sizeof(_enable)) < 0)
//		throw(error(SETSOCKOPT, port));
//	if (bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
//		throw(error(BIND, port));
//	if (listen(_fd, SOMAXCONN) < 0)
//		throw(error(LISTEN, port));
//	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == ERROR)
//		throw(error(FCNTL, port));
//}


void	Server::output() {
	//cout << "acceptFd: " << getAcceptFd() << endl;
	//cout << "kq: " << getKq() << endl;
	//cout << "fd or sockfd: " << getSockFd() << endl;
}
