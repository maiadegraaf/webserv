#include "Server.hpp"
#include "ServerIO.hpp"
#include "webserv.h"

vector<Config> extractServer(const string& filename)
{
    ConfigParser    confParser(filename);
    vector<Config>  conf;
    int             end;
	int 			start = confParser.findServer(0, &end);
	while(start > -1)
    {
        ConfigParser CPTmp;
        CPTmp.setServerContent(confParser.subVector(start, end));
        Config  tmp(CPTmp);
        conf.push_back(tmp);
		start = confParser.findServer(end, &end);
	}
    if (conf.empty())
        failure("Could not identify server content");
    return (conf);
}

int main(int argc, char **argv, char **envp)
{
	if (argc != 2)
		failure("Incorrect INPUT");
	vector<Config> conf = extractServer(argv[1]);
	ServerIO	webby(conf, returnContentType(), envp);
}
