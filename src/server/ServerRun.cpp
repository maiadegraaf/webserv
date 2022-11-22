#include "Server.hpp"

void Server::run()
{
	memset(_fds, 0 , sizeof(_fds));
	_fds[0].fd = _fd;
	_fds[0].events = POLLIN;
	while(1) {
		creatingPoll();
		loopFds();
	}
}

void Server::creatingPoll()
{
//	cout << "waiting poll..." << endl;
	if (poll(_fds, _nfds, -1) == 0)
		cerr << "poll() timed out.  End program." << endl;
}

void Server::loopFds()
{
	int     currentSize = _nfds;

	for (int i = 0; i < currentSize; i++) {
		if (_fds[i].revents == 0)
			continue;
		if (_fds[i].revents != POLLIN)
		{
//			cerr << "Error! revents = " <<  _fds[i].revents << endl;
//			perror("");
		}
		if (_fds[i].fd == _fd) {
			cerr << "Listening socket is readable" << endl;
			newConnection();
		}
		else if (!clientRequest(i))
			break ;
	}
	// lets create a vector of clients instead of this system
	//
	// recall
}

void Server::newConnection()
{
	do {
		_newFd = accept(_fd, NULL, NULL);
		if (_newFd < 0)
		{
			if (errno != EWOULDBLOCK)
				cerr << "accept() failed" << endl;
		}
		cerr << "New incoming connection - " << _newFd << endl;
		_fds[_nfds].fd = _newFd;
		_fds[_nfds].events = POLLIN;
		_nfds++;
	} while (_newFd != -1);
}
