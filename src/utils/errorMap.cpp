#include "../../inc/webserv.h"

map<int, string> setupErrorPages()
{
	map<int,string> errorPages;

	errorPages[400] = "default/error/400.html";
	errorPages[403] = "default/error/403.html";
	errorPages[404] = "default/error/404.html";
	errorPages[405] = "default/error/405.html";
	errorPages[418] = "default/error/418.html";
	return errorPages;
}
