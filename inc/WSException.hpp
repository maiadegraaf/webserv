#ifndef WSEXCEPTION_HPP
#define	WSEXCEPTION_HPP

#include <exception>

class WSException {
	public:
		class BadRequest : public std::exception {
			public: const char	*what() const throw() 			{ return "400 Bad Request"; }
		};
		class Forbidden : public std::exception {
			public: const char *what() const throw() 			{ return "403 Forbidden"; }
		};
		class PageNotFound : public std::exception {
			public: const char *what() const throw() 			{ return "404 Page Not Found";	}
		};
		class MethodNotAllowed : public std::exception {
			public: const char	*what() const throw() 			{ return "405 Method Not Allowed"; }
		};
		class PayloadTooLarge: public std::exception {
			public: const char *what() const throw() 			{ return "413 Payload Too Large"; }
		};
		class Imateapot : public std::exception {
			public: const char *what() const throw() 			{ return "418 I'm a teapot"; }
		};
		class InternalServerError : public std::exception {
			public: const char *what() const throw() 			{ return "500 Internal Server Error"; }
		};
		class HTTPVersionNotAvailable : public std::exception {
			public: const char *what() const throw() 			{ return "505 HTTP Version Not Available"; }
		};
};

#endif