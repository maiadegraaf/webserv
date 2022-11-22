#include "Server.hpp"

void Server::run()
{
	creatingKqueue();
	while(1) {
		newEvent();
		loopEvent();
	}
}

void Server::newEvent()
{
	_new_events = kevent(_kq, NULL, 0, _event, 1, NULL);
	if (_new_events == -1)
	{
		perror("kevent");
		exit(1);
	}
}

void Server::creatingKqueue()
{
	cout << "waiting kqueue..." << endl;
	_kq = kqueue();
	EV_SET(_change_event, _fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	if (kevent(_kq, _change_event, 1, NULL, 0, NULL) == -1)
	{
		perror("kevent");
		exit(1);
	}
}

void Server::loopEvent()
{
	for (int i = 0; _new_events > i; i++) {
		printf("amount of new events: %d\n", _new_events);
		_event_fd = _event[i].ident;

		// When the client disconnects an EOF is sent. By closing the file
		// descriptor the _event is automatically removed from the kqueue.
		if (_event[i].flags & EV_EOF) {
			printf("Client has disconnected\n");
			close(_event_fd);
			// remove client
		}
			// If the new _event's file descriptor is the same as the listening
			// socket's file descriptor, we are sure that a new client wants 
			// to connect to our socket.
		else if (_event_fd == _fd) {
			printf("New connection coming in...\n");

			// Incoming socket connection on the listening socket.
			// Create a new socket for the actual connection to client.
			_newFd = accept(_event_fd, (struct sockaddr *) &_client_addr, (socklen_t * ) & _len);
			if (_newFd == -1) {
				perror("Accept socket error");
			}

			// Put this new socket connection also as a 'filter' _event
			// to watch in kqueue, so we can now watch for events on this
			// new socket.
			EV_SET(_change_event, _newFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
			if (kevent(_kq, _change_event, 1, NULL, 0, NULL) < 0) {
				perror("kevent error");
			}
		} else if (_event[i].filter & EVFILT_READ) {
			clientRequest();
		}
	}
}

