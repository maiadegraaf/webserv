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
		if (event.flags & EV_EOF || event.flags == 1)
			this->disconnectClient(event.udata);
		else if (_sockFdIdxMap.find(_eventFd) != _sockFdIdxMap.end())
			this->connectNewClient();
//<<<<<<< HEAD
		if (event.filter == EVFILT_READ)
			this->incomingRequest(event);
//=======
		else if (event.filter == EVFILT_READ)
			this->incomingRequest(event.udata);
//>>>>>>> 00aa9a1c8f87f22366b3b794de5b5bee85cc9667
		else if (event.filter == EVFILT_WRITE)
			this->outgoingResponse(event.udata);
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

//<<<<<<< HEAD
//void	ServerIO::setupClientWrite(Client *client) {
//	struct kevent	newEvents[2];
//
//	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_DISABLE, 0, 0, client);
//	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_ENABLE, 0, 0, client);
//	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
//		perror("kevent client write");
//}
//
//void	ServerIO::setupClientRead(Client *client) {
//	struct kevent	newEvents[2];
//
//	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_ENABLE, 0, 0, client);
//	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_DISABLE, 0, 0, client);
//	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
//		perror("kevent client read");
//}
//
//void	ServerIO::setupClientEOF(Client *client) {
//	struct kevent	newEvents[2];
//
//	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_EOF, 0, 0, client);
//	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_EOF, 0, 0, client);
//	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
//		perror("kevent client EOF");
//}

//=======
void	ServerIO::setupClientWrite(Client *client) {
	struct kevent	newEvents[2];
//	struct timespec ts;
//
//	ts.tv_sec = 10;

	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_DISABLE, 0, 0, client);
	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_ENABLE , 0, 0, client);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
		perror("kevent client write");
}

void	ServerIO::setupClientRead(Client *client) {
	struct kevent	newEvents[2];
//>>>>>>> 00aa9a1c8f87f22366b3b794de5b5bee85cc9667
//	struct timespec ts;
//
//	ts.tv_sec = 10;

//<<<<<<< HEAD
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
//=======
	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_ENABLE, 0, 0, client);
	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_DISABLE, 0, 0, client);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
		perror("kevent client read");
}

void	ServerIO::setupClientEOF(Client *client) {
	struct kevent	newEvents[2];

	EV_SET(&newEvents[0], client->getSockFd(), EVFILT_READ, EV_EOF, 0, 0, client);
	EV_SET(&newEvents[1], client->getSockFd(), EVFILT_WRITE, EV_EOF, 0, 0, client);
	if (kevent(getKq(), newEvents, 2, NULL, 0, NULL) < 0)
		perror("kevent client EOF");
}

void	ServerIO::incomingRequest(void *udata) {
	Client *client = reinterpret_cast<Client *>(udata);

	if (client) {
		cerr << "\nServerIO::incomingRequest() : new request comming in" << endl;
		client->requestReceived(_envp);
		this->setupClientWrite(client);
//>>>>>>> 00aa9a1c8f87f22366b3b794de5b5bee85cc9667
		cerr <<  endl;
	}
	else
		perror("unknown request");
}

void	ServerIO::outgoingResponse(void *udata) {
	Client	*client = reinterpret_cast<Client *>(udata);
//<<<<<<< HEAD
	if (!client) {
		perror("unknown response");
		return ;
	}
	if (event.flags & EV_EOF)// || client->getClientMode() == request)
		disconnectClient(udata);
	else if (client->getClientMode() == response) {
		if (client->responseSend() == false) // nog maken
			client->setClientMode(request);
//=======
//	if (client) {
//		if (client->responseSend() == false )// nog maken
//			this->setupClientRead(client);
//>>>>>>> 00aa9a1c8f87f22366b3b794de5b5bee85cc9667
	}
	else
		perror("unkown response");
}

// Output
void ServerIO::output() {
	std::cout << "kq : " << _kq << std::endl;
}
