#include "Request.hpp"
// Constructor initializes attributes to 0 by default 
Request::Request()
	: _method(NULL), _dir(NULL), _protocol(NULL) {}
 
Request::Request( const Request& rhs ) {
	*this = rhs;
}

Request::~Request() {}

Request&	Request::operator=( const Request& rhs )
{
	this->_dir = rhs._dir;
	this->_method = rhs._method;
	this->_protocol = rhs._protocol;
	this->_content = rhs._content;
	this->_input = rhs._input;
	return *this;
}

Request::Request(string input) {
	setInput(input);
	setAttributes();
	setContent();
}

// Getters 
string Request::getMethod() { return _method; }
string Request::getDir() { return _dir; }
string Request::getProtocol() { return _protocol; }
string Request::getContentValue(string key) { return _content[key]; }

// Setters 
void Request::setMethod(string new_method) { _method = new_method; }
void Request::setDir(string new_dir) { _dir = new_dir; }
void Request::setProtocol(string new_protocol) { _protocol = new_protocol; }
void Request::setContentValue(string key, string value) { _content[key] = value; }

void Request::setInput(string input) {
	string vecPush;
	size_t start = 0;
	size_t findNL = input.find('\n', start);

	while (findNL != string::npos)
	{
		vecPush = input.substr(start, findNL - start);
//		cout << vecPush << "-- test --" << endl;
		_input.push_back(vecPush);
		start = findNL + 1;
		findNL = input.find('\n', start);
	}
}

void Request::setAttributes() {
	string			iString = _input[0];
	stringstream 	ss(iString);
	string 			tmp;

	getline(ss, tmp, ' ');
	setMethod(tmp);
	getline(ss, tmp, ' ');
	setDir(tmp);
	getline(ss, tmp);
	setProtocol(tmp);
}

void Request::setContent() {
	string key;
	string value;
	size_t idx;
	for (size_t i = 1; i < _input.size(); i++) {
		idx = _input[i].find(':', 0);
		key = _input[i].substr(0, idx);
		value = _input[i].substr(idx + 2, _input[i].size());
		setContentValue(key, value);
	}
}

// Output
void Request::output() {
	std::cout << "_method : " << _method << std::endl;
	std::cout << "_dir : " << _dir << std::endl;
	std::cout << "_protocol : " << _protocol << std::endl;
}
