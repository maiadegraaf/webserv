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
	_kq = kqueue();
	for (size_t idx = 0; idx < getServerSize(); idx++) {
		_server[idx].setupKq(getKq());
		_sockFdIdxMap[_server[idx].getSockFd()] = idx;
	}
}

void	ServerIO::newEvent() {
	_nrEvents = kevent(getKq(), NULL, 0, _events, 2, NULL);
	if (_nrEvents == -1) {
		perror("kevent error");
		exit(1);
	}
}

void	ServerIO::loopEvent( ) {
	struct kevent	event;

	for (int i = 0; i < _nrEvents; i++) {
		event = _events[i];
		_eventFd = event.ident;
		if (event.flags & EV_ERROR)
			cerr << "client got deleted" << endl;
		else if (_sockFdIdxMap.find(_eventFd) != _sockFdIdxMap.end())
			this->connectNewClient();
		else if (event.flags & EV_EOF) {
			this->disconnectClient(event);
			break ;
		}
		else if (event.filter == EVFILT_READ)
			this->incomingRequest(event);
		else if (event.filter == EVFILT_WRITE)
			this->outgoingResponse(event);
	}
}

void	ServerIO::disconnectClient(const struct kevent &event) {
	Client *client = static_cast<Client *>(event.udata);
	delete client;
	close(event.ident);
}

void	ServerIO::connectNewClient() {
	size_t			idx;

	idx = _sockFdIdxMap[_eventFd];
	_server[idx].clientNewAcceptFd(_eventFd);
	_server[idx].bindServerAcceptFdWithClient();
}


void	ServerIO::incomingRequest(const struct kevent &event) {
	Client *client = static_cast<Client *>(event.udata);
	if (!client) {
		perror("client is empty, no request");
		return ;
	}
	else if (client->getClientMode() == request) {
		client->requestReceived(_envp);
		if (client->getClientMode() == disconnect)
			this->disconnectClient(event);
	}
}

void	ServerIO::outgoingResponse(const struct kevent &event) {
	Client	*client = static_cast<Client *>(event.udata);
	if (!client) {
		perror("outgoing unknown response");
		return ;
	}
	else if (client->getClientMode() == response) {
		client->responseSend();
		if (client->getClientMode() == disconnect)
			this->disconnectClient(event);
	}
}

// Output
void ServerIO::output() {
	std::cout << "kq : " << _kq << std::endl;
}
