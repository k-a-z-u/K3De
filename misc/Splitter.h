#ifndef SPLITTER_H
#define SPLITTER_H

#include <vector>
#include <string>

class Splitter {

public:

	/** helper method to split strings by whitespaces while respecting quoted regions "..." */
	static std::vector<std::string> split(const std::string& line) {

		int state = 0;
		std::string str;
		std::vector<std::string> res;

		for (char c : line) {

			if (state == 0) {
				if (c == '"')	{state = 1; str = ""; continue;}
				if (c == ' ')	{continue;}
				if (c == '\t')	{continue;}
				if (c == '\n')	{continue;}
				if (c == '\r')	{continue;}
				                {state = 1; str = "";}
			}

			if (state == 1) {
				if (c == '"')	{state = 0; res.push_back(str); continue;}
				if (c == ' ')	{state = 0; res.push_back(str); continue;}
				if (c == '\t')	{state = 0; res.push_back(str); continue;}
				if (c == '\n')	{state = 0; res.push_back(str); continue;}
				if (c == '\r')	{state = 0; res.push_back(str); continue;}
				str += c;
			}

		}

		if (state == 1) {
			res.push_back(str);
		}

		return res;

	}

};

#endif // SPLITTER_H

