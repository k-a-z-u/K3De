#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <iomanip>

#if defined(WITH_DEBUG)
    #define Debug(sys, str) std::cout << "[" << std::setw(10) << sys << "] " << str << std::endl;
#else
    #define Debug(sys, str)
#endif

//class Debug {

//public:

//	static void print(const std::string& sys, const std::string& str) {
//		std::cout << "[" << sys << "] " << str << std::endl;
//	}

//};

#endif // DEBUG_H
