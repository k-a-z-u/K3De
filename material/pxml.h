#ifndef PXML_H
#define PXML_H

#include <unordered_map>
#include <vector>

struct PXMLNode {

	std::string name;
	std::unordered_map<std::string, std::string> attributes;
	std::vector<PXMLNode> children;

	PXMLNode() : name() {;}
	PXMLNode(const std::string& name) : name(name) {;}

	std::string getAttribute(const std::string& name) const {
		auto it = attributes.find(name);
		if (it == attributes.end()) {throw "error";}
		return it->second;
	}

	const std::string& getName() const {
		return name;
	}

};

#endif // PXML_H
