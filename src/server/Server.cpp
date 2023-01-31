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
	this->setAddr();
	this->setupSockFd();
	this->setupSocketOpt();
	this->setupNonBlock();
	this->bindSocket();
	this->listenSocket();
	_len = sizeof(_client_addr);
}

void	Server::setupSockFd() {
<<<<<<< Updated upstream
=======
	int new_fd;
>>>>>>> Stashed changes
	_sockFd = socket(_addrInfo->ai_family, _addrInfo->ai_socktype, _addrInfo->ai_protocol);
	if (_sockFd < 0) {
		cerr << "could not create socket (server)" << endl;
		exit(-1);
	}
	new_fd = accept(_sockFd, (struct sockaddr *) &_addrInfo->ai_addr, &_addrInfo->ai_addrlen);
	if (new_fd < 0)
		perror("accept");
	_sockFd = new_fd;
}

void	Server::setupSocketOpt() {
	int on = 1;

	if (setsockopt(getSockFd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
		perror("setsockopt");
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
	struct addrinfo hints;
<<<<<<< Updated upstream

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	hints.ai_protocol = 0;          /* Any protocol */
=======
	int status;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;    /* For wildcard IP address */
	hints.ai_protocol = 0;
>>>>>>> Stashed changes
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
//	if (getaddrinfo(_conf->getServerName().begin()->c_str(), _conf->getAddress().c_str(), &hints, &_addrInfo) < 0) {
<<<<<<< Updated upstream
	getaddrinfo("127.0.0.1", "8080", &hints, &_addrInfo);
//		perror("getaddrinfo");
//	}
}

void	Server::bindSocket() {
	if (bind(getSockFd(), _addrInfo->ai_addr, _addrInfo->ai_addrlen) < 0) {
=======
	if ((status = getaddrinfo("127.0.0.1", _conf->getAddress().c_str(), &hints, &_addrInfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
//	for (addrinfo* tmp = _addrInfo; tmp != nullptr; tmp = tmp->ai_next) {
//		perror("getaddrinfo");
//	}
	int accept(int s, struct sockaddr *addr, int *address_len);
//	}
}

void	Server::bindSocket() {
	if (bind(getSockFd(), _addrInfo->ai_addr, (int)_addrInfo->ai_addrlen) < 0) {
		perror("bind");
>>>>>>> Stashed changes
		cerr << "Error binding socket to local address" << endl;
		close(getSockFd());
		exit(-1);
	}
}

void	Server::listenSocket() {
	if (listen(getSockFd(), 32) < 0) {
		perror("listen");
		close(getSockFd());
		exit(-1);
	}
	freeaddrinfo(_addrInfo);
}

void	Server::output() {
	//cout << "acceptFd: " << getAcceptFd() << endl;
	//cout << "kq: " << getKq() << endl;
	//cout << "fd or sockfd: " << getSockFd() << endl;
}