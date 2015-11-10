#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>
#include <fstream>

#include <KLib/Assertions.h>

class FileHelper {

public:

	/** read the given file into a string */
	static std::string readFile(const std::string& filename) {

		std::string code;
		std::ifstream file(filename.c_str(), std::ios::in);

		_assertFalse(file.bad(), "error while opening file " + filename);
		_assertTrue(file.is_open(), "error while opening file " + filename);

		std::string line;
		while(getline(file, line)) {code += line + "\n";}
		file.close();

		return code;

	}

};

#endif // FILEHELPER_H
