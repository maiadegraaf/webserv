#include "Server.hpp"

void Server::run()
{
	creatingKqueue();
	while(1) {
		newEvent();
		loopEvent();
	}
}

void Server::creatingKqueue()
{
	cout << "waiting kqueue..." << endl;
	_kq = kqueue();
	cerr << " _kq " << _kq << endl;
	EV_SET(&_change_event[0], _fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
//	EV_SET(&_change_event[1], _fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	cerr << " _fd " << _fd << endl;
	if (kevent(_kq, _change_event, 1, NULL, 0, NULL) == -1)
	{
		perror("kevent");
		exit(1);
	}
}

void Server::newEvent()
{
//	EV_SET(&_event[0], _fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
//	EV_SET(&_event[1], _fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	_new_events = kevent(_kq, NULL, 0, _event, 2, NULL);
	if (_new_events == -1)
	{
		perror("kevent");
		exit(1);
	}
}

void Server::loopEvent()
{
	for (int i = 0; _new_events > i; i++) {
//		printf("amount of new events: %d\n", _new_events);
		_event_fd = _event[i].ident;

		// When the client disconnects an EOF is sent. By closing the file
		// descriptor the _event is automatically removed from the kqueue.
		if (_event[i].flags & EV_EOF) {
			printf("Client has disconnected\n");
			close(_event_fd);
			// remove client
		}
		if (_event_fd == _fd) {
		// If the new _event's file descriptor is the same as the listening
		// socket's file descriptor, we are sure that a new client wants
			printf("New connection coming in...\n");

			// Incoming socket connection on the listening socket.
			// Create a new socket for the actual connection to client.
			_newFd = accept(_event_fd, (struct sockaddr *) &_client_addr, (socklen_t * ) & _len);
			int opt_value = 1;
			setsockopt(_event_fd, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));
			if (_newFd == -1) {
				perror("Accept socket error");
			}

			// Put this new socket connection also as a 'filter' _event
			// to watch in kqueue, so we can now watch for events on this
			// new socket.
			struct kevent new_events[2];
//			Class
			EV_SET(&new_events[0], _newFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
			EV_SET(&new_events[1], _newFd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
			if (kevent(_kq, new_events, 2, NULL, 0, NULL) < 0) {
				perror("kevent error");
				cerr << errno << " --> errno\n";
			}
		}
		if (_event[i].filter == EVFILT_READ) {
			cerr << "do the request\n";
			clientRequest();
		}
	}
}

