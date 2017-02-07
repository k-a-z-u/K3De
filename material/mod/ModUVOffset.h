#ifndef MODUVOFFSET_H
#define MODUVOFFSET_H

#include <string>

class ModUVOffset {

protected:

	ModUVOffset* previous = nullptr;

public:

	virtual std::string getHeaderCode(const std::string& context) = 0;

	virtual std::string getMainCode(const std::string& context) = 0;

	/** input UV is either a function or a variable describing a vec2() */
	virtual std::string getModified(const std::string& context, const std::string& inpUV) = 0;

	virtual void addVariables(ShaderParams& params) = 0;

	void setPrevious(ModUVOffset* prev) {this->previous = prev;}

};


#endif // MODUVOFFSET_H
