#include "includes/webserv.h"

void failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		failure("Incorrect INPUT");
    configParser    conPar;
	conPar.openFile(argv[1]);
}
