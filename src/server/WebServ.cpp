#include "WebServ.hpp"
// Constructor initializes attributes to 0 by default 

WebServ::WebServ( const WebServ& rhs) {
	*this = rhs;
}

WebServ&	WebServ::operator=( const WebServ& rhs ) {
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

//
void	WebServ::newEvent() {
	_nrEvents = kevent(getKq(), NULL, 0, _events, 2, NULL);
	if (_nrEvents == -1) {
		perror("kevent");
		exit(1);
	}
}

void	WebServ::loopEvent( ) {
	struct kevent	event;

	for (int i = 0; i < _nrEvents; i++) {
		event = _events[i];
		_eventFd = event.ident;
		if (event.flags == EV_EOF)
			this->disconnectClient(event.udata);
		else if (_sockFdIdxMap.find(_eventFd) != _sockFdIdxMap.end())
			this->connectNewClient();
		else if (event.filter == EVFILT_READ)
			this->incomingRequest(event.udata);
	}
}

void	WebServ::disconnectClient(void *udata) {
	printf("Client has disconnected\n");
	close(_eventFd);
	Client *client = static_cast<Client *>(udata);
	delete client;
}

void	WebServ::connectNewClient() {
	size_t			idx;

	printf("New connection coming in...\n");
	idx = _sockFdIdxMap[_eventFd];
	_server[idx].clientNewAcceptFd(_eventFd);
	_server[idx].bindServerAcceptFdWithClient();
}

void WebServ::incomingRequest(void *udata) {
	Client *client = reinterpret_cast<Client *>(udata);
	if (client) {
		cerr << "doing the request\n";
		cerr << "eventFd :" << _eventFd << endl;
		client->clientRequest();
	}
}

// Output
void WebServ::output() {
	std::cout << "kq : " << _kq << std::endl;
}
