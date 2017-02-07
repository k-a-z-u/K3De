#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <iomanip>

#define Debug(sys, str) std::cout << "[" << std::setw(10) << sys << "] " << str << std::endl;

//class Debug {

//public:

//	static void print(const std::string& sys, const std::string& str) {
//		std::cout << "[" << sys << "] " << str << std::endl;
//	}

//};

#endif // DEBUG_H
