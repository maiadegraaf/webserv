#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "WSException.hpp"

using namespace std;

class Request
{
	/* *******************
	 * (Con/De)structors *
	 * *******************/
	public:
		Request();
		Request( const Request &rhs );
		~Request();
		Request& operator=( const Request &rhs );
//		Request(string input);

	/* ************
	 * Attributes *
	 * ************/
	private:
		string				_method;
		string				_dir;
		string				_protocol;
		vector<string>		_input;
		map<string, string> _header; // maybe call header
		string 				_buffer,
							_body;
		bool 				_requestHeader,
							_requestBody;
//							_setWrite;

	/* *********
	 * Getters *
	 * *********/
	public:
		string			getMethod()							{ return _method; }
		string			getDir() 							{ return _dir; }
		string			getProtocol()						{ return _protocol; }
		string			getHeaderValue(string key)			{ return _header[key]; }
		vector<string>	getInput()							{ return _input; }
		string 			getBody()							{ return _body; }

	/* *********
	 * Setters *
	 * *********/
	public:
		void	setMethod(string new_method)				{ this->_method = new_method; }
		void	setDir(string new_dir)						{ this->_dir = new_dir; }
		void	setProtocol(string new_protocol)			{ this->_protocol = new_protocol; }
		void	setHeaderValue(string key, string value)	{ this->_header[key] = value; }
		void 	setRequestHeader(bool newBool)				{ this->_requestHeader = newBool; }
		void 	setRequestBody(bool newBool)				{ this->_requestBody = newBool; }

		void 	parseHeader();
		void 	setHeader();
		void 	parseBody();
		void	setAttributes();
		void	setContent();

	/* ********
	 * Output *
	 * ********/
        void	output();

};
 
#endif
