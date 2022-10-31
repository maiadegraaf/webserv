#include <iostream>
#include <sys/socket.h>

int main() {
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		perror("cannot create socket");
		return 0;
	}
	std::cout << "HHELO" << socket_fd << std::endl;
	return 0;
}
