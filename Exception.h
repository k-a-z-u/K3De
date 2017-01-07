#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

class Exception : public std::exception {

private:

	std::string msg;

public:

	/** ctor */
	Exception(const std::string& msg) : msg(msg) {;}

	const char* what() const throw() {
		return msg.c_str();
	}

};

#endif // EXCEPTION_H
