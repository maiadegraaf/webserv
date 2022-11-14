#include "webserv.h"



//int main(int argc, char **argv)
//{
//	if (argc != 2)
//		failure("Incorrect INPUT");
//	config	conf(argv[1]);
//}

//#include <iostream>
//#include <sys/socket.h>
//#include "Request.hpp"

int main() {
	try {
		string str = "GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1\nHost: code.tutsplus.com\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nAccept-Language: en-us,en;q=0.5\nAccept-Encoding: gzip,deflate\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\nKeep-Alive: 300\nConnection: keep-alive\nCookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\nPragma: no-cache\nCache-Control: no-cache\n";
		Request	req(str);

		req.output();
		cout << req.getContentValue("Host") << " -" << endl;
		cout << req.getContentValue("User-Agent") << " -" << endl;
		cout << req.getContentValue("Accept") << " -" << endl;
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}
	try {
		string str = "GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1\nHost: code.tutsplus.com\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nAccept-Language: en-us,en;q=0.5\nAccept-Encoding: gzip,deflate\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\nKeep-Alive: 300\nConnection: keep-alive\nCookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\nPragma: no-cache\nCache-Control: no-cache\n\nBody of text\n";
		Request	req(str);
		cout << req.getFile() << endl;
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}
	return 0;
}


//int main(int argc, char *argv) {
	// config parsing
	// seeting up server/sockets/
	// connnectieng with clients
	// handling requests
	// sending repsonse
//}

