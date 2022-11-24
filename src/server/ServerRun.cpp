#include "Server.hpp"

void Server::run()
{
	creatingKqueue();
//	int i = 0;
	while(1) {
		loopEvent();
//		i++;
//		i = i % 2;
	}
}

void Server::creatingKqueue()
{
//	struct kevent			_change_event[2];
	cout << "waiting kqueue..." << endl;
	_kq = kqueue();
	EV_SET(&_change_event[0], _fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	EV_SET(&_change_event[1], _fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	EV_SET(&_event[0], _fd, EVFILT_READ, EV_ENABLE, 0, 0, 0);
	EV_SET(&_event[1], _fd, EVFILT_WRITE, EV_ENABLE, 0, 0, 0);
	_new_events = kevent(_kq, _change_event, 2, _event, 2, NULL);
	if (_new_events == -1)
	{
		perror("kevent");
		exit(1);
	}
//	newEvent();
}


//void Server::newEvent()
//{
//	EV_SET(&_event[0], _fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
//	EV_SET(&_event[1], _fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
//	_new_event = kevent(_kq, NULL)
//	if (_new_events == -1)
//	{
//		perror("kevent");
//		exit(1);
//	}
//}

void Server::loopEvent()
{
//	struct kevent			_event[2];

//		printf("amount of new events: %d\n", _new_events);
	_event_fd = _event[0].ident;

	// When the client disconnects an EOF is sent. By closing the file
	// descriptor the _event is automatically removed from the kqueue.
	if (_event[0].flags & EV_EOF) {
		printf("Client has disconnected\n");
		close(_event_fd);
//		 delete client class;
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
		cout << "Accepted new client fd" << endl;

		// Put this new socket connection also as a 'filter' _event
		// to watch in kqueue, so we can now watch for events on this
		// new socket.
		EV_SET(&_event[0], _newFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		EV_SET(&_event[1], _newFd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		cout << "EV_SET event" << endl;
		if (kevent(_kq, NULL, 0, _event, 2, NULL) < 0) {
			perror("kevent error");
		}
	} else if (_event[0].filter & EVFILT_READ) {
		clientRequest();
	}
}

