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
