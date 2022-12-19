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
	cout << "initializing kqueue..." << endl;
	_kq = kqueue();
	for (size_t idx = 0; idx < getServerSize(); idx++) {
		_server[idx].setupKq(getKq());
		_sockFdIdxMap[_server[idx].getSockFd()] = idx;
	}
}

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
		if (event.flags & EV_EOF)
			this->disconnectClient(event.udata);
		else if (_sockFdIdxMap.find(_eventFd) != _sockFdIdxMap.end())
			this->connectNewClient();
		else if (event.filter == EVFILT_READ)
			this->incomingRequest(event.udata);
		else if (event.filter == EVFILT_WRITE)
			this->outgoingResponse(event.udata);
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

	cerr << "WebServ::connectNewClient() : New connection coming in..." << endl;
	idx = _sockFdIdxMap[_eventFd];
	_server[idx].clientNewAcceptFd(_eventFd);
	_server[idx].bindServerAcceptFdWithClient();
	idx = _sockFdIdxMap[_eventFd];
	cerr << "WebServ::connectNewClient() : Client connected with server " << endl;
}

void	WebServ::setupClientWrite(Client *client) {
	struct kevent	newEvents[2];

	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_DISABLE, 0, 0, client);
	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_ENABLE, 0, 0, client);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
		perror("kevent client write");
}

void	WebServ::setupClientRead(Client *client) {
	struct kevent	newEvents[2];

	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_ENABLE, 0, 0, client);
	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_DISABLE, 0, 0, client);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
		perror("kevent client read");
}

void	WebServ::setupClientEOF(Client *client) {
	struct kevent	newEvents[2];

	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_EOF, 0, 0, client);
	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_EOF, 0, 0, client);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
		perror("kevent client EOF");
}

void	WebServ::incomingRequest(void *udata) {
	Client *client = reinterpret_cast<Client *>(udata);
	if (client) {
		cerr << "\nWebServ::incomingRequest() : new request comming in" << endl;
		if (client->requestReceived() == true)
			this->setupClientWrite(client);
		else if (client->getRequestMode() == false)
			this->setupClientEOF(client);
		cerr <<  endl;
	}
	else
		perror("unknown request");
}

void	WebServ::outgoingResponse(void *udata) {
	Client	*client = reinterpret_cast<Client *>(udata);
	if (client) {
		if (client->responseSend() == false) // nog maken
			this->setupClientRead(client);
	}
	else
		perror("unkown response");
}

// Output
void WebServ::output() {
	std::cout << "kq : " << _kq << std::endl;
}
