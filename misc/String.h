#ifndef STRING_H
#define STRING_H

namespace String {

	/** replace the first occurence of from with to */
	static inline bool replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if(start_pos == std::string::npos) {return false;}
		str.replace(start_pos, from.length(), to);
		return true;
	}

};

#endif // STRING_H
