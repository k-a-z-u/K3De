#ifndef SHADERPARAMS_H
#define SHADERPARAMS_H



#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "../Exception.h"
#include "../textures/ITexture.h"
#include "ShaderVariable.h"
#include "../shader/ShaderVersion.h"

class ShaderParams {

	struct Variable {
		int index;
		std::string name;
		std::string code;
		int useCount = 0;	// how often this variable is used
		Variable(const int index, const std::string& name, const std::string& code, const int useCount) :
			index(index), name(name), code(code), useCount(useCount) {;}
	};

	struct Function {
		int index;
		std::string name;
		std::string code;
		std::vector<std::string> usedVariables;
		std::vector<std::string> usedFunctions;
		int useCount = 0;
		Function(const int index, const std::string& name, const std::string& code, const std::vector<std::string>& usedVariables, const std::vector<std::string>& usedFunctions, const int useCount) :
			index(index), name(name), code(code), usedVariables(usedVariables), usedFunctions(usedFunctions), useCount(useCount) {;}
	};

protected:

	std::unordered_map<std::string, Variable> variables;
	std::unordered_map<std::string, Function> functions;

	std::string name;
	std::string mainPre;
	std::string main;

public:

	// TODO: ensure order?

	void setName(const std::string& name) {
		this->name = name;
	}

	void addVariable(const ShaderVariable& sv) {
		_addVariable(sv.getName(), sv.getDecl() + ";");
	}

	/** add a new variable with the given name and declaration */
	void _addVariable(const std::string& name, const std::string& decl, const int useCount = 0) {
		if (variables.find(name) != variables.end()) {return;}
		const Variable var = Variable(variables.size(), name, decl, useCount);
		variables.insert(std::pair<std::string, Variable>(name, var));
	}

	/** mark the variable with the given name as "used" */
	void usedVariable(const std::string& name) {
		auto it = variables.find(name);
		if (it == variables.end()) {throw Exception("error. variable not found: " + name);}
		++it->second.useCount;
	}

	/** is a variable named "name" presend AND used? */
	bool hasUsedVariable(const std::string& name) const {
		return hasVariable(name) && (variables.find(name)->second.useCount);
	}

	bool hasVariable(const std::string& name) const {
		return variables.find(name) != variables.end();
	}

	void addFunction(const std::string& name, const std::string decl, const std::vector<std::string>& usedVariables, const std::vector<std::string>& usedFunctions, const int useCount = 1) {
		if (functions.find(name) != functions.end()) {return;}
		const Function func = Function(functions.size(), name, decl, usedVariables, usedFunctions, useCount);
		functions.insert(std::pair<std::string, Function>(name, func));
	}

	bool hasFunction(const std::string& name) const {
		return functions.find(name) != functions.end();
	}

	/** mark the function with the given name as "used" */
	void usedFunction(const std::string& name) {
		auto it = functions.find(name);
		if (it == functions.end()) {throw Exception("error. function not found: " + name);}
		++it->second.useCount;
		for (const std::string& varName : it->second.usedVariables) {
			usedVariable(varName);
		}
		for (const std::string& funcName : it->second.usedFunctions) {
			usedFunction(funcName);
		}
	}

	void addMainPre(const std::string& code) {
		mainPre += code;
	}
	void addMainPreLine(const std::string& code) {
		mainPre += "\t" + code + "\n";
	}

	void addMain(const std::string& code) {
		main += code;
	}
	void addMainLine(const std::string& code, const int indent = 1) {
		for (int i = 0; i < indent; ++i) {main += "\t";}
		main += code + "\n";
	}
	void addMainSection(const std::string& name, const int indent = 1) {
		main += "\n";
		for (int i = 0; i < indent; ++i) {main += "\t";}
		main += "// " + name + "\n";
	}

protected:

	std::string section(const std::string& name) const {
		return "\n// ------------------------ " + name + " ------------------------\n";
	}

	std::string getHeader() const {
		return ShaderVersion::getGLSL();
	}

	std::string getVariables() const {

		// convert to vector
		std::vector<Variable> vars;
		for (const auto it : variables) {vars.push_back(it.second);}

		// sort the vector by variable index
		auto comp = [] (const Variable& a, const Variable& b) {return a.index < b.index;};
		std::sort(vars.begin(), vars.end(), comp);

		// insert
		std::stringstream ss;
		ss << section("variables");
		for (const Variable& var : vars) {
			if (var.useCount == 0) {ss << "// UNUSED: ";}
			ss << var.code << "\n";
		}
		return ss.str();

	}

	std::string getFunctions() const {

		// convert to vector
		std::vector<Function> funcs;
		for (const auto it : functions) {funcs.push_back(it.second);}

		// sort the vector by variable index
		auto comp = [] (const Function& a, const Function& b) {return a.index < b.index;};
		std::sort(funcs.begin(), funcs.end(), comp);

		std::stringstream ss;
		ss << section("function");
		for (const Function& func : funcs) {
			ss << func.code << "\n";
		}
		return ss.str();
	}

	std::string getMain() const {
		return
			section("main") +
			"\nvoid main() {\n" +
		    mainPre + "\n\n" +
			main +
			"}";
	}

};

#endif // SHADERPARAMS_H
