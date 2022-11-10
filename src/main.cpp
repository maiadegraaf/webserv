#include "includes/webserv.h"



int main(int argc, char **argv)
{
	if (argc != 2)
		failure("Incorrect INPUT");
	config	conf(argv[1]);
}
