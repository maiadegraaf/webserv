#include "Server.hpp"

Server::Server(Config *conf)
		: _fd(-1), _acceptFd(-1), _conf(conf), _closeConnection(false) {
	this->setup();
//	this->run();
}

Server&	Server::operator=( const Server& rhs ) {
	this->_fd = rhs._fd;
	this->_kq = rhs._kq;
	this->_new_events = rhs._new_events;
	this->_len = rhs._len;
	this->_acceptFd = rhs._acceptFd;
	this->_event_fd = rhs._event_fd;
	this->_servAddr = rhs._servAddr;
	this->_client_addr = rhs._client_addr;
	this->_conf = rhs._conf;
	this->_change_event[0] = rhs._change_event[0];
	this->_change_event[1] = rhs._change_event[1];
	this->_event[0] = rhs._event[0];
	this->_event[1] = rhs._event[1];
	this->_contentType = rhs._contentType;
	this->_location = rhs._location;
	this->_closeConnection = rhs._closeConnection;
	this->_maxSize = rhs._maxSize;

	return *this;
}

void Server::setup() {
	int on = 1;

	setMaxSize(static_cast<size_t>(_conf->getMaxSize()));
	_contentType = returnContentType();
	_location = _conf->getLocation();
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0) {
		cerr << "could not create socket (server)" << endl;
		exit(-1);
	}
	if (setsockopt(_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
		cerr << "setsockopt() failed" << endl;
		close(_fd);
		exit(-1);
	}
	//makes the socket non blocking
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) {
		cerr << "fcntl failed: to make the socket unblocking" << endl;
		close(_fd);
		exit(-1);
	}
	this->setAddr();
	if(bind(_fd, (struct sockaddr*) &_servAddr, sizeof(_servAddr)) < 0)
	{
		cerr << "Error binding socket to local address" << endl;
		close(_fd);
		exit(-1);
	}
	if (listen(_fd, 32) < 0)
	{
		cerr << "Listen failed" << endl;
		close(_fd);
		exit(-1);
	}
	_len = sizeof(_client_addr);
}

void Server::setAddr()
{
	bzero((char*)&_servAddr, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr.sin_port = htons(_conf->getAddress());
}

void	Server::setupKq(int kq) {
	EV_SET(&_change_event[0], _fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
//	EV_SET(&_change_event[1], _fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	// if (kevent(kq, _change_event, 2, NULL, 0, NULL) == -1)
	if (kevent(kq, _change_event, 1, NULL, 0, NULL) == -1)
	{
		perror("kevent");
		exit(1);
	}
}

int	Server::clientAcceptFd(int eventFd) {
	// Incoming socket connection on the listening socket.
	// Create a new socket for the actual connection to client.
	_acceptFd = accept(eventFd, (struct sockaddr *) &_client_addr, (socklen_t * ) & _len);
	int opt_value = 1;
	setsockopt(eventFd, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));
	if (_acceptFd == -1) {
		perror("Accept socket error");
	}
	setsockopt(eventFd, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));
	return _acceptFd;
	// Put this new socket connection also as a 'filter' _event
	//			// to watch in kqueue, so we can now watch for events on this
	//			// new socket.
}

void	Server::output() {
	cout << "acceptFd: " << _acceptFd << endl;
	cout << "kq: " << _kq << endl;
	cout << "fd or sockfd: " << _fd << endl;
}
