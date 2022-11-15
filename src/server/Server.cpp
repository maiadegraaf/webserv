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
 
Server::~Server() {}

Server&	Server::operator=( const Server& rhs )
{
	(void)rhs;
//	this->_fd = rhs._fd;
//	this->_servAddr = rhs._servAddr;
//	this->_conf = rhs._conf;
////	this->_fds = rhs._fds;
//	this->_len = rhs._len;
//	this->_nfds = rhs._nfds;
//	this->_newFd = rhs._newFd;
	return *this;
}

void Server::output(){}

std::ifstream::pos_type Server::filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void Server::setup()
{
    int on = 1;

    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
    {
        std::cerr << "could not create socket (server)" << std::endl;
        exit(-1);
    }
    if (setsockopt(_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
    {
        perror("setsockopt() failed");
        close(_fd);
        exit(-1);
    }
    //makes the socket non blocking
    if (ioctl(_fd, FIONBIO, (char *)&on) < 0)
    {
        std::cerr << "ioctl failed: to make the socket unblocking" << std::endl;
        close(_fd);
        exit(-1);
    }
    this->setAddr();
    if(bind(_fd, (struct sockaddr*) &_servAddr, sizeof(_servAddr)) < 0)
    {
        std::cerr << "Error binding socket to local address" << std::endl;
        close(_fd);
        exit(-1);
    }
    if (listen(_fd, 32) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
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
    int     rc;
    char    buffer[80];
    int     close_conn, end_server = FALSE;


    memset(_fds, 0 , sizeof(_fds));
    _fds[0].fd = _fd;
    _fds[0].events = POLLIN;
    int timeout = (3 * 60 * 1000);
    do  {
        std::cout << "waiting poll..." << std::endl;
        if (poll(_fds, _nfds, timeout) == 0)
        {
            printf("  poll() timed out.  End program.\n");
            break;
        }
        int current_size = _nfds;
        for (int i = 0; i < current_size; i++) {
            if (_fds[i].revents == 0)
                continue;
            if (_fds[i].revents != POLLIN)
            {
                printf("  Error! revents = %d\n", _fds[i].revents);
                end_server = true;
                break;
            }
            if (_fds[i].fd == _fd) {
                printf("  Listening socket is readable\n");
                do {
                    _newFd = accept(_fd , NULL, NULL);
                    if (_newFd < 0) 
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            perror("  accept() failed");
                            end_server = true;
                        }
                        break;
                    }
                    printf("  New incoming connection - %d\n", _newFd);
                    _fds[_nfds].fd = _newFd;
                    _fds[_nfds].events = POLLIN;
                    _nfds++;
                } while (_newFd != -1);
            } else {
                printf("  Descriptor %d is readable\n", _fds[i].fd);
                close_conn = FALSE;
                while (1) 
                {
                    rc = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
                    if (rc < 0) 
                    {
                        if (errno != EWOULDBLOCK) 
                        {
                            perror("  recv() failed");
                            close_conn = true;
                        }
                        break;
                    }
                    if (rc == 0) 
                    {
                        printf("  Connection closed\n");
                        close_conn = true;
                        break;
                    }
                    _len = rc;
                    printf("  %d bytes received\n", _len);
//                    send(_fds[i].fd, buffer, _len, 0);
                }
                int read = open("www/index.html", O_RDONLY);
                off_t _len = filesize("www/index.html");
                if (sendfile(read, _fds[i].fd, 0, &_len, NULL, 0) < 0) 
                {
                    perror("  send() failed");
                    close_conn = true;
                    break;
                }
                if (close_conn)
                {
                    close(_fds[i].fd);
                    _fds[i].fd = -1;
                }
            }
        }
    } while (end_server == false);
}

void Server::closeFds()
{
    for (int i = 0; i < _nfds; i++) {
        if (_fds[i].fd >= 0)
            close(_fds[i].fd);
    }
}