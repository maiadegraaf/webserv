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

void	Server::setKqRead() {
	EV_SET(&_changeEvent[0], getSockFd(), EVFILT_READ, EV_ENABLE, 0, 0, 0);
	EV_SET(&_changeEvent[1], getSockFd(), EVFILT_WRITE, EV_DISABLE, 0, 0, 0);
	if (kevent(getKq(), _changeEvent, 2, NULL, 0, NULL) == -1) {
		perror("kevent server read error");
		exit(1);
	}
}

void	Server::setKqWrite() {
	EV_SET(&_changeEvent[0], getSockFd(), EVFILT_READ, EV_DISABLE, 0, 0, 0);
	EV_SET(&_changeEvent[1], getSockFd(), EVFILT_WRITE, EV_ENABLE, 0, 0, 0);
	if (kevent(getKq(), _changeEvent, 2, NULL, 0, NULL) == -1) {
		perror("kevent server write error");
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
	EV_SET(&newEvents[0], getAcceptFd(), EVFILT_READ, EV_ADD, 0, 0, newClient);
	EV_SET(&newEvents[1], getAcceptFd(), EVFILT_WRITE, EV_ADD, 0, 0, newClient);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0) {
		perror("kevent error");
	}
}
