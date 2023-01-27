#include "ServerIO.hpp"
#include <sys/select.h>

ServerIO::ServerIO( const ServerIO& rhs) {
	*this = rhs;
}

ServerIO&	ServerIO::operator=( const ServerIO& rhs ) {
	(void)rhs;
	return *this;
}

ServerIO::ServerIO(vector<Config> newConfig, map<string, string> newContentType, char** envp)
	:_config(newConfig), _contentType(newContentType), _envp(envp) {
	size_t	size = _config.size();

	for (size_t i = 0; i < size; i++) {
		Server	tmp(&_config[i]);
		_server.push_back(tmp);
	}
	_serverSize = size;
	runServerIO();
}

void	ServerIO::runServerIO() {
	initKq();
	while(1) {
		newEvent();
		loopEvent();
	}
}

void	ServerIO::initKq() {
	cout << "initializing kqueue..." << endl;
	_kq = kqueue();
	for (size_t idx = 0; idx < getServerSize(); idx++) {
		_server[idx].setupKq(getKq());
		_sockFdIdxMap[_server[idx].getSockFd()] = idx;
	}
}

void	ServerIO::newEvent() {
	_nrEvents = kevent(getKq(), NULL, 0, _events, 2, NULL);
	if (_nrEvents == -1) {
		perror("kevent");
		exit(1);
	}
}

void	ServerIO::loopEvent( ) {
	struct kevent	event;

	for (int i = 0; i < _nrEvents; i++) {
		event = _events[i];
		_eventFd = event.ident;
		if (event.flags & EV_ERROR)
			errx(EXIT_FAILURE,	"Event error: %s", strerror(event.data));
		else if (event.flags & EV_ERROR)
			cerr << "client got deleted" << endl;
		if (_sockFdIdxMap.find(_eventFd) != _sockFdIdxMap.end())
			this->connectNewClient();
		if (event.filter == EVFILT_READ)
			this->incomingRequest(event);
		else if (event.filter == EVFILT_WRITE)
			this->outgoingResponse(event);
	}
}

void	ServerIO::disconnectClient(void *udata) {
	printf("Client has disconnected\n");
	close(_eventFd);
	Client *client = static_cast<Client *>(udata);
	delete client;
}

void	ServerIO::connectNewClient() {
	size_t			idx;

	cerr << "ServerIO::connectNewClient() : New connection coming in..." << endl;
	idx = _sockFdIdxMap[_eventFd];
	_server[idx].clientNewAcceptFd(_eventFd);
	_server[idx].bindServerAcceptFdWithClient();
	idx = _sockFdIdxMap[_eventFd];
	cerr << "ServerIO::connectNewClient() : Client connected with server " << endl;
}


void	ServerIO::incomingRequest(struct kevent event) {
	void *udata = event.udata;
	Client *client = reinterpret_cast<Client *>(udata);
	if (!client) {
		perror("unknown request");
		return ;
	}
	if (event.flags & EV_EOF)//|| client->getClientMode() == response)
		disconnectClient(udata);
	else if (client->getClientMode() == request) {
		cerr << "\nServerIO::incomingRequest() : new request comming in" << endl;
		client->requestReceived(_envp);
		cerr <<  endl;
	}
}

void	ServerIO::outgoingResponse(struct kevent event) {
	Client	*client = reinterpret_cast<Client *>(event.udata);
	if (!client) {
		perror("unknown response");
		return ;
	}
	if (event.flags & EV_EOF)// || client->getClientMode() == request)
		disconnectClient(udata);
	else if (client->getClientMode() == response) {
		if (client->responseSend() == false) // nog maken
			client->setClientMode(request);

}

// Output
void ServerIO::output() {
	std::cout << "kq : " << _kq << std::endl;
}
