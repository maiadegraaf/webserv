#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#define BUFF_SIZE 300000
#define PORT 8080

void failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int main() {
	// create socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		failure("cannot create socket");
	std::cout << "Socket Number: " << socket_fd << std::endl;

	// create socket file descriptor
	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		failure("setsockopt failure");
	struct sockaddr_in address;
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// bind socket
	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		failure("bind failure");

	// listen
	if (listen(socket_fd, 10) < 0)
		failure("listen failure");

	int	new_fd;
	long valread;
	char *msg = "Server says hi";
	while(true)
	{
		std::cout << std::setfill('+') << std::setw(10) << "" << " Waiting for a new connection " << std::setfill('+') << std::setw(10) << "" << std::endl;
		new_fd = accept(socket_fd, (struct sockaddr *)&address, sizeof(address));
		if (new_fd < 0)
			failure("accept failure");
		char buffer[BUFF_SIZE] = {0};
		valread = read(new_fd, buffer, BUFF_SIZE);
		std::cout << buffer << std::endl;
		write (new_fd, msg, strlen(msg));
		std::cout << std::setfill('-') << std::setw(10) << "" << " Message Sent " << std::setfill('-') << std::setw(10) << "" << std::endl;
		close(new_fd);
	}
	return EXIT_SUCCESS;
}
