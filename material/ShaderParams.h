#ifndef SHADERPARAMS_H_OLD
#define SHADERPARAMS_H_OLD

#include <string>
#include <sstream>
#include <unordered_map>

class ShaderParamsOLD {

private:

	std::unordered_map<std::string, std::string> variables;
	std::unordered_map<std::string, std::string> functions;

public:

	// TODO: ensure order?

	void addVariable(const std::string& name, const std::string& decl) {
		if (variables.find(name) != variables.end()) {return;}
		variables[name] = decl;
	}

	void addFunction(const std::string& name, const std::string decl) {
		if (functions.find(name) != functions.end()) {return;}
		functions[name] = decl;
	}


public:

	std::string getVariables() const {
		std::stringstream ss;
		for (const auto it : variables) {
			ss << it.second << "\n";
		}
		return ss.str();
	}

	std::string getFunctions() const {
		std::stringstream ss;
		for (const auto it : functions) {
			ss << it.second << "\n";
		}
		return ss.str();
	}

};

#endif // SHADERPARAMS_H_OLD
