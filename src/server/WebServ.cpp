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
//	runWebServ();
}

void	WebServ::runWebServ() {
	initKq();
	while(1) {
		for (size_t i = 0; i < getServerSize(); i++) {
			newEvent(i);
			loopEvent(i);
		}
	}
}

void	WebServ::initKq() {
	cout << "waiting kqueue..." << endl;
	_kq = kqueue();
	cerr << "_kq = " << _kq << endl;
	for (size_t i = 0; i < getServerSize(); i++) {
		_server[i].setupKq(getKq());
	}
}

void	WebServ::newEvent(size_t idx) {
	_newEvents = kevent(getKq(), NULL, 0, _server[idx].getEvent(), 2, NULL);
	if (_newEvents == -1)	{
		perror("kevent");
		exit(1);
	}
}

void	WebServ::loopEvent(size_t idx) {
	struct kevent	event;
	int				acceptFd;
	for (int i = 0; i < _newEvents; i++) {
		event = _server[idx].getEventByIndex(i);
		_eventFd = event.ident;

		// When the client disconnects an EOF is sent. By closing the file
		// descriptor the _event is automatically removed from the kqueue.
		if (event.flags == EV_EOF) {
			printf("Client has disconnected\n");
			close(_eventFd);
			Client *client = static_cast<Client *>(event.udata);
			delete client;
		}
		else if (_eventFd == _server[i].getSockFd()) {
			// If the new _event's file descriptor is the same as the listening
			// socket's file descriptor, we are sure that a new client wants
			printf("New connection coming in...\n");
			acceptFd = _server[i].getAcceptFd(_eventFd);
			Client *newClient = new Client(acceptFd, _server[i].getLocation(), _contentType, _server[i].getMaxSize()); // used _newFD instead of _event_fd here
			struct kevent newEvents[2];
			EV_SET(&newEvents[0], acceptFd, EVFILT_READ, EV_ADD, 0, 0, newClient);
			EV_SET(&newEvents[1], acceptFd, EVFILT_WRITE, EV_ADD, 0, 0, newClient);
			if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0) {
				perror("kevent error");
//				cerr << errno << " --> errno\n";
			}
		}
		else if (event.filter == EVFILT_READ) {
			cerr << "doing the request\n";
			Client *client = static_cast<Client *>(event.udata);
			client->clientRequest();
//			clientRequest(); // comment this out to test with your version alfred
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

