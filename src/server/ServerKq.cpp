#include "Server.hpp"

void	Server::setupKq(int kq) {
//	EV_SET(&_changeEvent[1], getSockFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
//	if (kevent(kq, _changeEvent, 2, NULL, 0, NULL) == -1)
	EV_SET(&_changeEvent[0], getSockFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	if (kevent(kq, _changeEvent, 1, NULL, 0, NULL) == -1)
	{
		perror("kevent");
		exit(1);
	}
	_kq = kq;
}

void	Server::clientNewAcceptFd(int eventFd) {
	int opt_value = 1;
	setsockopt(eventFd, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));
	_acceptFd = accept(eventFd, (struct sockaddr *) &_client_addr, (socklen_t * ) & _len);
	if (_acceptFd == -1)
		perror("Accept socket error");
//	setsockopt(eventFd, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));
}

void	Server::bindServerAcceptFdWithClient() {
	struct kevent	newEvents[2];

	Client *newClient = new Client(getAcceptFd(), getLocation(), _conf->getRoot(), _conf->getErrorPage(), getContentType(), getMaxSize());
	EV_SET(&newEvents[0], getAcceptFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, newClient);
	EV_SET(&newEvents[1], getAcceptFd(), EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, newClient);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0) {
		perror("kevent error");
	}
}
