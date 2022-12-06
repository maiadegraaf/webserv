#include "Client.hpp"
// Constructor initializes attributes to 0 by default


Client::Client(int newSockFD, map<string,string> newLocation, map<string, string> newContentType)
		: _sockFD(newSockFD), _location(newLocation), _contentType(newContentType), _strRequest("") {

}

Client&	Client::operator=( const Client& rhs ) {
	(void)rhs;
	return *this;
}

// Output
void	Client::output() {
	std::cout << "SockFD : " << _sockFD << std::endl;
//	std::cout << "fds : " << _fds << std::endl;
//	std::cout << "location : " << _location << std::endl;
	std::cout << "strRequest : " << _strRequest << std::endl;
}

void	Client::clientRequest() {
	string strRequest = receiveStrRequest();
//	cerr << strRequest << endl;
	if (strRequest.empty())
		return ;
	try {
		Request	clientReq(strRequest);
		handleRequest(clientReq);
	} catch (exception &e) {
		string tmpMessage(e.what());
		Response error(tmpMessage, _sockFD, getContentType("html"));
		error.sendResponse();
	}
}

string	Client::receiveStrRequest() {
	int     rc;
	char    buffer[100];
	string	request("");
	string	tmp;

	while (1) {
		rc = recv(_sockFD, buffer, sizeof(buffer), 0);
//		cerr << buffer << endl;
		if (rc < 0) {
//			cerr << errno << " --> errno\n";
//			if (errno != EWOULDBLOCK) {
			cerr << "recv() stopped reading " << endl;
			this->setCloseConnection(true);
//			}
			break;
		}
		if (rc == 0) {
			cerr << "  Connection closed" << endl;
			this->setCloseConnection(true);
			break;
		}
		_len = rc;
		tmp.assign(buffer, rc);
		cerr << _len << " bytes received " << endl;
		request.append(tmp);
	}
	return request;
}

void	Client::handleRequest(Request clientReq) { // should we use a --> const Request &ref ??
	string confFile;
	string file;
	string filePath("www/");

	confFile = getLocation(clientReq.getDir());
	if (!confFile.empty())
		file.append(confFile); // page not foudn exception
	else {
		if (clientReq.getDir().empty())
			throw Request::BadRequestException();
		file.append(clientReq.getDir()); // Response
	}
	if (clientReq.getMethod().compare("GET") == 0)
		handleGetRequest(file, filePath);
	if (clientReq.getMethod().compare("POST") == 0)
		handlePostRequest(file, filePath, clientReq);
//	else if (clientReq.getMethod().compare("DELETE") == 0)
//	{
//
//	}

}

void Client::handleGetRequest(string file, string filePath)
{
	string contentType;
	string extension;

	filePath.append(file); // exception filePath;
	extension = filePath.substr(filePath.find_last_of('.') + 1);
	if (extension.compare("php") == 0) {
		handleCGIResponse(filePath, _contentType["html"]);
		if (getCloseConnection() == true)
			return ;
		return ;
	}
	contentType = _contentType[extension];
	if (!contentType.empty()) {
		handleResponse(filePath, contentType);
		if (getCloseConnection() == true)
			return ;
	} else
		throw PageNotFoundException();
	return ; // not a supported extension
}
//
//void Client::setPostContent(string input, int i) {
//	_postContent.push_back(vector<string>());
//
//}

void Client::handlePostRequest(string file, string filepath, Request clientReq)
{
//	string line;
//	string len;
//	string disp;
//	string cont_type;
//	string filename;
//	string savePath;
	(void )filepath;
	(void )file;
	string type = clientReq.getContentValue("Content-Type");

	if (type.compare("text/plain") == 0)
		parsePostPlainRequest(clientReq);
	else if (type.compare("application/x-www-form-urlencoded") == 0)
		parsePostWwwRequest(clientReq);
	else if (type.compare("multipart/form-data") == 0)
		parsePostMultipartRequest(clientReq);
}

void	Client::handleResponse(string filePath, string contentType) {
//	cerr << "this is file ---> " << file << "  - this is  content type --> " << confFile<<  endl;
	off_t _len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, _sockFD, _len);
	if (!clientResponse.sendResponse())
		setCloseConnection(true);
	_strRequest.clear();
}

void	Client::handleCGIResponse(string filePath, string contentType) {
//	cerr << "this is file ---> " << file << "  - this is  content type --> " << confFile<<  endl;
	off_t _len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, _sockFD, _len); // different lenght constructor for cgi
	if (!clientResponse.sendResponse())
		setCloseConnection(true);
	_strRequest.clear();
}
