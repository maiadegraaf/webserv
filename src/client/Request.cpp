#include "Request.hpp"

Request::Request()
        : _method(""), _dir(""), _protocol(""), _file(""), _buffer(""), _body(""),
		_requestHeader(false), _requestBody(false) {}

Request::Request( const Request& rhs ) {
    *this = rhs;
}

Request::~Request() {}

Request&	Request::operator=( const Request& rhs ) {
    this->_dir = rhs._dir;
    this->_method = rhs._method;
    this->_protocol = rhs._protocol;
    this->_header = rhs._header;
    this->_input = rhs._input;
	this->_file = rhs._file;
	this->_buffer = rhs._buffer;

	return *this;
}
//
//Request::Request(string input) {
//    setInput(input);
//    setAttributes();
//    setContent();
//}

// Setters
void	Request::setInput(string input) {
    string vecPush;
    stringstream ss(input);

    while (getline(ss, vecPush, '\n')) {
        _input.push_back(vecPush);
    }
}

bool	Request::appendBuffer(string recvBuffer) {
	_buffer.append(recvBuffer);

	if (this->getRequestHeader() == true) {
		this->parseHeader();
		return true;
	}
	else if (this->getRequestBody() == true) {
		this->parseBody();
		return true;
	}
	return false;
}

void	Request::parseHeader() {
	stringstream	ss(_buffer);
	string			tmp;

	while (getline(ss, tmp)) {
		if (tmp.find("\r") == string::npos) {
			_buffer = tmp;
			return ;
		}
		else if (!tmp.compare("\r")) {
			this->setHeader();
			return ;
		}
		_input.push_back(tmp);
	}
}
void	Request::setHeader() {
	this->setAttributes();
	this->setContent();
	if ((_content.find("Content-Length") != _content.end() && \
			getContentValue("Content-Length").compare("0")) && !getMethod().compare("POST")) {
		this->setRequestBody(true);
	}
	this->setRequestHeader(false);
	_input.clear();
	_buffer.clear();
}

void	Request::parseBody() {
	stringstream	ss(_buffer);
	string			tmp;

	while (getline(ss, buffer)) {
		if (tmp.find("\r") == string::npos) {
			_buffer = tmp;
			return ;
		}
		else if (tmp.compare("\r")) {
			this->setRequestBody(false);
			return ;
		}
		_body.append(tmp);
		_body.append("\n");
	}
}

void	Request::setAttributes() {
    stringstream 	ss(_input[0]);
    string 			tmp;

    getline(ss, tmp, ' ');
    if (!tmp.empty() && !tmp.compare("GET") || !tmp.compare("POST") || \
	!tmp.compare("DELETE")) {
        throw WSException::MethodNotAllowed();
    }
    setMethod(tmp);
    getline(ss, tmp, ' ');
    if (!tmp.empty())
        throw WSException::BadRequest();
    setDir(tmp);
    getline(ss, tmp);
    if (!tmp.empty())
        throw WSException::BadRequest();
	if (!tmp.compare("HTTP/1.1")) {
		cerr << " http version "  << tmp << endl;
		throw WSException::HTTPVersionNotAvailable();
	}
    setProtocol(tmp);
}

void	Request::reset() {
	Request	newRequest;

	this = newRequest;
}

//
//void Request::setContent() {
//    string	key;
//    string	value;
//    size_t	idx;
//    size_t	i = 1;
//    size_t	size = _input.size();
//
//    while (i < size) { // && _input[i].compare(0, 1,"\n")) {
//        idx = _input[i].find(':', 0);
//        if (idx == string::npos)
//            break ;
//        key = _input[i].substr(0, idx);
//        value = _input[i].substr(idx + 2);
//        setContentValue(key, value);
//        i++;
//    }
//    _file.clear();
//    for (size_t j = i; j < size; j++) {
//        _file.append(_input[j]);
//        _file.append("\n");
//    }
//}

// Output
//void Request::output() {
//    std::cout << "_method : " << _method << std::endl;
//    std::cout << "_dir : " << _dir << std::endl;
//    std::cout << "_protocol : " << _protocol << std::endl;
//}
