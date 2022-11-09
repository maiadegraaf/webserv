#include "Request.hpp"
// Constructor initializes attributes to 0 by default 
Request::Request()
	: _method(""), _dir(""), _protocol(""), _file("") {}
 
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
	this->_file = rhs._file;
	return *this;
}

Request::Request(string input) {
	setInput(input);
	setAttributes();
	setContent();
}

// Setters
void Request::setInput(string input) {
	string vecPush;
	stringstream ss(input);

	while (getline(ss, vecPush, '\n')) {
		_input.push_back(vecPush);
	}
}

void Request::setAttributes() {
	string			iString = _input[0];
	stringstream 	ss(iString);
	string 			tmp;

	getline(ss, tmp, ' ');
	if (!tmp.compare(0, 1, "") && \
	!tmp.compare(0, 4, "GET") && tmp.compare(0, 5, "POST") \
	&& tmp.compare(0, 7, "DELETE")) {
		throw BadRequestException();
	}
	setMethod(tmp);
	getline(ss, tmp, ' ');
	if (!tmp.compare(0, 1, ""))
		throw BadRequestException();
	setDir(tmp);
	getline(ss, tmp);
	if (!tmp.compare(0, 1, ""))
		throw BadRequestException();
	setProtocol(tmp);
}

void Request::setContent() {
	string	key;
	string	value;
	size_t	idx;
	size_t	i = 1;
	size_t	size = _input.size();

	while (i < size && _input[i].compare(0, 1,"")) {
		idx = _input[i].find(':', 0);
		key = _input[i].substr(0, idx);
		value = _input[i].substr(idx + 2, _input[i].size());
		if (idx + 2 > _input[i].size())
			throw BadRequestException();
		setContentValue(key, value);
		i++;
	}
	_file.clear();
	if (i == size)
		return ;
	if (_input[i].compare(0, 1, ""))
		throw BadRequestException();
	for (size_t j = i; j < size; j++) {
		_file.append(_input[j]);
		_file.append("\n");
	}
}

// Output
void Request::output() {
	std::cout << "_method : " << _method << std::endl;
	std::cout << "_dir : " << _dir << std::endl;
	std::cout << "_protocol : " << _protocol << std::endl;
}
