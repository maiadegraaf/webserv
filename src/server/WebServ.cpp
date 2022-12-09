#include "WebServ.hpp"
// Constructor initializes attributes to 0 by default 

WebServ::WebServ( const WebServ& rhs)
{
	*this = rhs;
}

WebServ&	WebServ::operator=( const WebServ& rhs )
{
	(void)rhs;
	return *this;
}

WebServ::WebServ(vector<Config> newConfig, map<string, string> newContentType)
	:_config(newConfig), _contentType(newContentType) {
	size_t	size = _config.size();

	for (size_t i = 0; i < size; i++) {
		Server	tmp(&_config[i]);
		_server.push_back(tmp);
	}
	_serverSize = size;
	runWebServ();
}

void	WebServ::runWebServ() {
	initKq();
	while(1) {
//		for (size_t idx = 0; idx < getServerSize(); idx++) {
//			newEvent(idx);
////			loopEvent(idx);
//		}
//		for (size_t idx = 0; idx < getServerSize(); idx++) {
////			newEvent(idx);
//			loopEvent(idx);
//		}
		newEvent();
		loopEvent();
	}
}

void	WebServ::initKq() {
	cout << "waiting kqueue..." << endl;
	_kq = kqueue();
	cerr << "_kq = " << _kq << endl;
	for (size_t idx = 0; idx < getServerSize(); idx++) {
		_server[idx].setupKq(getKq());
		_sockFdIdxMap[_server[idx].getSockFd()] = idx;
	}
}

//void	WebServ::newEvent(size_t idx) {
////	EV_SET(&_server[idx].getEventByIndex(0), _server[idx].getSockFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
////	EV_SET(&(_server[idx].getEventByindex(1)), _server[idx].getSockFd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
////	_nrEvents = kevent(getKq(), NULL, 0, _server[idx].getEvent(), 2, NULL);
//	_nrEvents = kevent(getKq(), NULL, 0, _event), 2, NULL);
//	if (_nrEvents == -1)	{
//		perror("kevent");
//		exit(1);
//	}
////	_sockFdIdxMap[getSockFd()] = idx;
////	_server[idx].setNrEvents(_nrEvents);
//}

//
void	WebServ::newEvent() {
//	EV_SET(_server[idx].getEventByIndex(0), idx, EVFILT_READ, EV_ADD | EV_ENBLE, 0, 0, 0);
//	EV_SET(_server[idx].getEventByindex(0), idx, EVFILT_WRITE, EV_ADD | EV_ENBLE, 0, 0, 0);
	_nrEvents = kevent(getKq(), NULL, 0, _events, 2, NULL);
	if (_nrEvents == -1)	{
		perror("kevent");
		exit(1);
	}
}

//
//void	WebServ::loopEvent() {
//	struct kevent	event;
//	int				acceptFd;
//	size_t			idx;
////	cout << "this is idx:" << idx << ": this is newEvents:" << _newEvents <<":\n";
//	for (int i = 0; i < _newEvents; i++) {
//		event = _event[i];
//		_eventFd = event.ident;
//
//		// When the client disconnects an EOF is sent. By closing the file
//		// descriptor the _event is automatically removed from the kqueue.
//		if (event.flags == EV_EOF) {
//			printf("Client has disconnected\n");
//			close(_eventFd);
//			Client *client = static_cast<Client *>(event.udata);
//			delete client;
//		}
//		else if (_fdIdxMap.find(_eventFd) != _fdIdxMap.end()) {
//			// If the new _event's file descriptor is the same as the listening
//			// socket's file descriptor, we are sure that a new client wants
//			printf("New connection coming in...\n");
//			idx = _fdIdxMap[_eventFd];
//			acceptFd = _server[idx].clientAcceptFd(_eventFd);
//			cerr << "this is acceptFd:" << acceptFd << ":\n";
//			Client *newClient = new Client(acceptFd, _server[idx].getLocation(), _contentType, _server[idx].getMaxSize()); // used _newFD instead of _event_fd here
//			struct kevent newEvents[2];
//			EV_SET(&newEvents[0], acceptFd, EVFILT_READ, EV_ADD, 0, 0, newClient);
//			EV_SET(&newEvents[1], acceptFd, EVFILT_WRITE, EV_ADD, 0, 0, newClient);
//			if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0) {
//				perror("kevent error");
//			}
//			_fdIdxMap[acceptFd] = idx;
////			newClient->output();
//		}
//		else if (event.filter == EVFILT_READ && _fdIdxMap.find(_eventFd) != _fdIdxMap.end() ) {//_eventFd == _server[idx].getNewFd()) {
//			Client *client = reinterpret_cast<Client *>(event.udata);
//			if (client) {
//				cerr << "doing the request\n";
//				cerr << "eventFd :" << _eventFd << endl;
//				_server[idx].output();
//				client->clientRequest();
//			}
//		}
//	}
//}


void	WebServ::loopEvent( ) {//size_t idx) {
	struct kevent	event;
	int				acceptFd;
	size_t			idx;
//	cout << "this is idx:" << idx << ": this is newEvents:" << _newEvents <<":\n";
//	_nrEvents = _server[idx].getNrEvents();
	for (int i = 0; i < _nrEvents; i++) {
//		event = _server[idx].getEventByIndex(i);
		event = _events[i];
		_eventFd = event.ident;

		// When the client disconnects an EOF is sent. By closing the file
		// descriptor the _event is automatically removed from the kqueue.
		if (event.flags == EV_EOF) {
			printf("Client has disconnected\n");
			close(_eventFd);
			Client *client = static_cast<Client *>(event.udata);
			delete client;
		}
		else if (_sockFdIdxMap.find(_eventFd) != _sockFdIdxMap.end()) {
//		else if (_eventFd == _server[idx].getSockFd()) {
			// If the new _event's file descriptor is the same as the listening
			// socket's file descriptor, we are sure that a new client wants
			idx = _sockFdIdxMap[_eventFd];
			printf("New connection coming in...\n");
			acceptFd = _server[idx].clientAcceptFd(_eventFd);
			cerr << "this is acceptFd:" << acceptFd << ":\n";
			Client *newClient = new Client(acceptFd, _server[idx].getLocation(), _contentType, _server[idx].getMaxSize()); // used _newFD instead of _event_fd here
			struct kevent newEvents[2];
			EV_SET(&newEvents[0], acceptFd, EVFILT_READ, EV_ADD, 0, 0, newClient);
			EV_SET(&newEvents[1], acceptFd, EVFILT_WRITE, EV_ADD, 0, 0, newClient);
			if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0) {
				perror("kevent error");
			}
			_acceptFdIdxMap[acceptFd] = idx;
//			newClient->output();
		}
		else if (event.filter == EVFILT_READ && _acceptFdIdxMap.find(_eventFd) != _acceptFdIdxMap.end()) {
//		else if (event.filter == EVFILT_READ && _eventFd == _server[idx].getAcceptFd()) {
				idx = _acceptFdIdxMap[_eventFd];
				Client *client = reinterpret_cast<Client *>(event.udata);
				if (client) {
					cerr << "doing the request\n";
					cerr << "eventFd :" << _eventFd << endl;
					_server[idx].output();
					client->clientRequest();
				}
		}
	}
}


// Output
void WebServ::output()
{
//  std::cout << "servers : " << _servers << std::endl;
  std::cout << "kq : " << _kq << std::endl;
//  std::cout << "contentType : " << _contentType << std::endl;
}
