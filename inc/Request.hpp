#ifndef REQUEST_HPP
#define REQUEST_HPP

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
		stringstream 		*_ss;
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
		void 			organizeParsingRequest();
		bool 			getRequestBody()					{ return this->_requestBody; }
		stringstream*	getSS()								{ return this->_ss; }

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
		void	setSS(stringstream *ss)						{ this->_ss = ss; }

	//		void	setInput(string input);
		bool	appendBuffer(string recvBuffer);
		void	setAttributes();
		void	setHeaderContent();
		void 	parseBufferBody();
		void 	parseBuffer();
		void 	setupHeader();
		bool	checkIfBody();
		bool	checkIfHeader();

	/* ********
	 * Output *
	 * ********/
        void	output();

};
 
#endif
