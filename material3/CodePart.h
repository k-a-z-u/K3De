#ifndef CODEPART_H
#define CODEPART_H

#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>

#include "VarType.h"

/** one line of code */
struct CodeLine {

	Var variable;

	std::string code;

	bool terminated = false;

	bool commentedOut = false;

	explicit CodeLine(const std::string& code) : variable(), code(code) {;}

	explicit CodeLine(const Var var, const std::string assigned) : variable(var), code(assigned) {;}

	explicit CodeLine(const Var var) : variable(var), terminated(true) {;}

	std::string get() const {
		return	(commentedOut ? "//" : "") +
				((variable.type == VarType::NONE) ? (code) : (variable.getAssigned(code))) +
				(terminated ? ";" : "") +
				"\r\n";
	}

	/** add ";" */
	CodeLine& terminate() {
		this->terminated = true;
		return *this;
	}

	void assignTo(Var var) {
		this->variable = var;
	}

	CodeLine& commentOut() {
		this->commentedOut = true;
		return *this;
	}

};

struct CodeLines {

	std::vector<CodeLine> lines;

	/** append a new line of code */
	CodeLines& operator += (const std::string& line) {
		lines.push_back(CodeLine(line));
		return *this;
	}

	/** append new lines of code */
	CodeLines& operator += (const std::vector<std::string>& lines) {
		for (const std::string& line : lines) {
			this->lines.push_back(CodeLine(line));
		}
		return *this;
	}

	/** append a new line of code */
	CodeLines& operator += (const CodeLine& line) {
		lines.push_back(line);
		return *this;
	}

	/** append a new line of code */
	CodeLines& operator += (const CodeLines& o) {
		for (const CodeLine& cl : o.lines) {
			lines.push_back(cl);
		}
		return *this;
	}



//	/** get all code lines */
//	const std::vector<CodeLine>& getLines() const {
//		return lines;
//	}

//	/** assign the last code line to the given variable */
//	CodeLines assignedTo(Var var) {
//		CodePart copy = *this;
//		copy.lines.back().variable = var;
//		return copy;
//	}

	void assignLastLineTo(Var var) {
		lines.back().variable = var;
	}

};

/** several lines of code. the last line contains the result */
struct CodePart {

	CodeLines header;

	std::vector<std::string> usedVariables;

	/** global variables needed/defined before main() */
	std::unordered_set<Var> globalVariables;

	CodeLines manualGlobals;

	CodeLines functions;

	/** code before the main code */
	CodeLines preMain;

	/** main code */
	CodeLines main;



	/** ctor */
	CodePart() {
		;
	}


	/** concat code parts */
	CodePart& operator += (const CodePart& code) {

		for (const Var v : code.globalVariables) {
			globalVariables.insert(v);
		}

		for (const std::string& s : code.usedVariables) {
			usedVariables.push_back(s);
		}

		header += code.header;
		manualGlobals += code.manualGlobals;
		functions += code.functions;
		preMain += code.preMain;
		main += code.main;

		return *this;

	}

//	void assignTo(Var var) {
//		main.assignLastLineTo(var);
//	}

//	CodePart assignedTo(Var var) {
//		CodePart copy = *this;
//		copy.main.assignLastLineTo(var);
//		return copy;
//	}



	/** track variable usage */
	void addUsage(const std::string& name) {
		usedVariables.push_back(name);
	}

	/** add a new global variable needed/defined by the code */
	void addGlobalVariable(Var v) {
		globalVariables.insert(v);
	}

	/** get the code as final multiline string */
	std::string asString(bool lineNumbers = false) const {

		int braces = 0;
		std::string res;


		// copy global variables (editable)
		std::vector<Var> globals;
		for (const Var& v : globalVariables) {globals.push_back(v);}

		// mark variables as used
		for (const std::string& name : usedVariables) {
			for (Var& v : globals) {
				if (v.name == name) {v.setUsed(); continue;}
			}
			std::cout << "variable not found: " << name << std::endl;
		}

		// combine all code parts
		std::vector<CodeLine> lines;

		// header
		for (const CodeLine& cl : header.lines)			{lines.push_back(cl);}

		// global variables
		for (const Var& v : globals) {
			CodeLine cl(v);
			if (!v.used) {cl.commentOut();}	// TODO
			lines.push_back(cl);
		}
		for (const CodeLine& cl : manualGlobals.lines)	{lines.push_back(cl);}

		// additional functions
		for (const CodeLine& cl : functions.lines)		{lines.push_back(cl);}

		// main
		lines.push_back(CodeLine("void main() {"));
		for (const CodeLine& cl : preMain.lines)		{lines.push_back(cl);}
		for (const CodeLine& cl : main.lines)			{lines.push_back(cl);}
		lines.push_back(CodeLine("}"));

		// create output
		int lineNr = 2;
		for (const CodeLine& cl : lines) {

			const std::string line = cl.get();

			if (!line.empty() && line[0] == '}') {--braces;}

			if (lineNumbers) {
				res += std::to_string(lineNr) + "\t";
				++lineNr;
			}

			// ident
			for (int i = 0; i < braces; ++i) {
				res += "    ";
			}

			res += cl.get();

			for (int i = 1; i < (int)line.length(); ++i) {
				const char c = line[i];
				if (c == '{') {++braces;}
				if (c == '}') {--braces;}
			}

		}

		return res;

	}

};

#endif // CODEPART_H
