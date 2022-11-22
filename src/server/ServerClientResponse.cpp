#include "Server.hpp"

bool	Server::clientRequest() { //wanneer keep alive ???? // segfault her in bool
	string strRequest = receiveStrRequest();
//	cerr << strRequest << endl;
	if (strRequest.empty())
		return false;
	try {
		Request	clientReq(strRequest);
		return (handleRequest(clientReq));
	} catch (exception &e) {
		string tmpMessage(e.what());
		Response error(tmpMessage, _event_fd, _contentType["html"]);
		error.sendResponse();
		this->setCloseConnection(true);
		return false;
	}
}

string Server::receiveStrRequest() {
	int     rc;
	char    buffer[80];
	string	request("");
	string	tmp;

	while (1) {
		rc = recv(_event_fd, buffer, sizeof(buffer), 0);
		if (rc < 0) {
			if (errno != EWOULDBLOCK) {
				cerr << "  recv() failed " << endl;
				this->setCloseConnection(true);
			}
			break;
		}
		if (rc == 0) {
			cerr << "  Connection closed" << endl;
			this->setCloseConnection(true);
			break;
		}
		_len = rc;
		tmp.assign(buffer, rc);
		cerr << _len << " bytes receive " << endl;
		request.append(tmp);
	}
	return request;
}

bool	Server::handleRequest(Request clientReq) { // should we use a --> const Request &ref ??
	string filePath("www/");
	string confFile;
	string file;
	string extension;
	string contentType;

	confFile = _conf->getLocation(clientReq.getDir());
	if (!confFile.empty())
		file.append(confFile); // page not foudn exception
	else {
		if (clientReq.getDir().empty())
			throw Request::BadRequestException();
		file.append(clientReq.getDir()); // Response
	}
	filePath.append(file); // exception filePath;
	extension = filePath.substr(filePath.find_last_of('.') + 1);
	if (extension.compare("php") == 0) {
		handleCGIResponse(filePath, _contentType["html"]);
		if (getCloseConnection() == true)
			return false;
		return true;
	}
	contentType = _contentType[extension];
	if (!contentType.empty()) {
		handleResponse(filePath, contentType);
		if (getCloseConnection() == true)
			return false;
	} else
		throw PageNotFoundException(); // not a supported extension
	if (this->getCloseConnection() == true) {
		close(_event_fd);
		_event_fd = -1;
	}
	return true;
}

void	Server::handleResponse(string filePath, string contentType) {
//	cerr << "this is file ---> " << file << "  - this is  content type --> " << confFile<<  endl;
	off_t _len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, _event_fd, _len);
	if (!clientResponse.sendResponse())
		setCloseConnection(true);
}

void	Server::handleCGIResponse(string filePath, string contentType) {
//	cerr << "this is file ---> " << file << "  - this is  content type --> " << confFile<<  endl;
	off_t _len = fileSize(filePath.c_str());
	Response	clientResponse(filePath, "200 OK", contentType, _event_fd, _len); // different lenght constructor for cgi
	if (!clientResponse.sendResponse())
		setCloseConnection(true);
}