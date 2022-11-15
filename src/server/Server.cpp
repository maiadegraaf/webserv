#include "Server.hpp"

Server::Server(Config *conf)
		: _fd(-1), _conf(conf), _nfds(1), _newFd(-1)
{
	this->setup();
	this->run();
	this->closeFds();
}

Server::Server( const Server& rhs)
{
	*this = rhs;
}

Server::~Server(){}

Server&	Server::operator=( const Server& rhs )
{
	(void)rhs;
	return *this;
}

void Server::output(){}

ifstream::pos_type Server::filesize(const char* filename)
{
	ifstream in(filename, ifstream::ate | ifstream::binary);
	return in.tellg();
}

void Server::setup()
{
	int on = 1;

	_contentType = returnContentType();
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
	{
		cerr << "could not create socket (server)" << endl;
		exit(-1);
	}
	if (setsockopt(_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		cerr << "setsockopt() failed" << endl;
		close(_fd);
		exit(-1);
	}
	//makes the socket non blocking
	if (ioctl(_fd, FIONBIO, (char *)&on) < 0)
	{
		cerr << "ioctl failed: to make the socket unblocking" << endl;
		close(_fd);
		exit(-1);
	}
	this->setAddr();
	if(bind(_fd, (struct sockaddr*) &_servAddr, sizeof(_servAddr)) < 0)
	{
		cerr << "Error binding socket to local address" << endl;
		close(_fd);
		exit(-1);
	}
	if (listen(_fd, 32) < 0)
	{
		cerr << "Listen failed" << endl;
		close(_fd);
		exit(-1);
	}
}

void Server::setAddr()
{
	bzero((char*)&_servAddr, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr.sin_port = htons(_conf->getAddress());
}

void Server::run()
{
	memset(_fds, 0 , sizeof(_fds));
	_fds[0].fd = _fd;
	_fds[0].events = POLLIN;
	while(1) {
		creatingPoll();
		loopFds();
	}
}

void Server::creatingPoll()
{
	int timeout = (3 * 60 * 1000);

	cout << "waiting poll..." << endl;
	if (poll(_fds, _nfds, timeout) == 0)
		cerr << "poll() timed out.  End program." << endl;
}

void Server::loopFds()
{
	int     current_size = _nfds;
	int     close_conn = false;

	for (int i = 0; i < current_size; i++) {
		if (_fds[i].revents == 0)
			continue;
		if (_fds[i].revents != POLLIN)
			cerr << "Error! revents = " <<  _fds[i].revents << endl;
		if (_fds[i].fd == _fd) {
			cerr << "Listening socket is readable" << endl;
			newConnection();
		}
		else
			clientRequest(i, close_conn); // break is not incorperated here.
	}
}

void	Server::clientRequest(int i, int close_conn) { //wanneer keep alive ????
	// new connection
	try {
		cerr << "Descriptor " << _fds[i].fd << " is readable" << endl;
		string	request = receiveRequest(i, close_conn);
		Request	clientReq(request);
		string 	filePath("www/");
		string	file(_conf->_location[clientReq.getDir()]);
		// if file does not exist throw exception

		filePath.append(file);
		string	extension = filePath.substr(filePath.find_lastOf('.') + 1);
		string	contentType = _contentType[extension];
		// if not extension throw error;

		string	message("200 OK");
		off_t _len = filesize(filePath);
		Response	clientResponse(filePath, message, contentType, _fds[i].fd, _len);
		Response.send();
		//		int read = open("www/index.html", O_RDONLY);
//		if (sendfile(read, _fds[i].fd, 0, &_len, NULL, 0) < 0)
//		{
//			cerr << "send() failed" << endl;
//			close_conn = true;
//			break; // break function needs to work
//		}
//		if (close_conn)
//		{
//			close(_fds[i].fd);
//			_fds[i].fd = -1;
//		}

	} catch (exception &e) {
		string tmpMessage(e.what());
		Response(tmpMessage, _fds[i].fd);
		Response.send();
	}
}

void Server::newConnection()
{
	do {
		_newFd = accept(_fd , NULL, NULL);
		if (_newFd < 0)
		{
			if (errno != EWOULDBLOCK)
				cerr << "accept() failed" << endl;
		}
		cerr << "New incoming connection - " << _newFd << endl;
		_fds[_nfds].fd = _newFd;
		_fds[_nfds].events = POLLIN;
		_nfds++;
	} while (_newFd != -1);
}

string Server::receiveRequest(int i, int &close_conn)
{
	int     rc;
	char    buffer[80];
	string	request("");

	while (1)
	{
		rc = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				cerr << "  recv() failed " << endl;
				close_conn = true;
			}
			break;
		}
		if (rc == 0)
		{
			cerr << "  Connection closed" << endl;
			close_conn = true;
			break;
		}
		_len = rc;
		cerr << _len << " bytes receive" << endl ;
		request.append(buffer);
	}
	return request;
}

void Server::closeFds()
{
	for (int i = 0; i < _nfds; i++) {
		if (_fds[i].fd >= 0)
			close(_fds[i].fd);
	}
}