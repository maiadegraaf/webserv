#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cstdlib>
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
		Request &operator=( const Request &rhs );
		Request(string input);

	/* ************
	 * Attributes *
	 * ************/
	private:
		string				_buffer;
		string				_method;
		string				_dir;
		string				_protocol;
		vector<string>		_input;
		map<string, string> _header;
		string 				_body;
		bool 				_requestHeader,
							_requestBody;

	/* *********
	 * Getters *
	 * *********/
	public:
		string			getMethod()							{ return this->_method; }
		string			getDir() 							{ return this->_dir; }
		string			getProtocol()						{ return this->_protocol; }
		string			getHeaderValue(string key)			{ return this->_header[key]; }
		vector<string>	getInput()							{ return this->_input; }
		string 			getBody()							{ return this->_body; }
		bool 			getRequestHeader()					{ return this->_requestHeader; }
		bool 			getRequestBody()					{ return this->_requestBody; }

	/* *********
	 * Setters *
	 * *********/
	public:
		void	setMethod(string new_method)				{ this->_method = new_method; }
		void	setDir(string new_dir)						{ this->_dir = new_dir; }
		void	setProtocol(string new_protocol)			{ this->_protocol = new_protocol; }
		void	setHeaderValue(string key, string value)	{ this->_header[key] = value; }
		void 	setRequestHeader(bool nBool)				{ this->_requestHeader = nBool; }
		void 	setRequestBody(bool nBool)					{ this->_requestBody = nBool; }
//		void	setInput(string input);
		bool	appendBuffer(string recvBuffer);
		void	setAttributes();
		void	setHeaderContent();
		void 	parseBufferBody();
		void 	parseBufferHeader();
		void 	setupHeader();

	/* ********
	 * Output *
	 * ********/
        void	output();

};
 
#endif
