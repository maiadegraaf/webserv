#include "includes/webserv.h"



int main(int argc, char **argv)
{
	if (argc != 2)
		failure("Incorrect INPUT");
    configParser    conPar;
	conPar.openFile(argv[1]);
}
