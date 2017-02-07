#ifndef XML_H
#define XML_H

#include "../lib/tinyxml2/tinyxml2.h"

using XMLDoc = tinyxml2::XMLDocument;
using XMLErr = tinyxml2::XMLError;
using XMLElem = tinyxml2::XMLElement;

#define FOREACH_ELEM(sub, root) \
for (XMLElem* sub = (XMLElem*)root->FirstChild(); sub != nullptr; sub = (XMLElem*)sub->NextSibling())

#define FOREACH_ELEM_NAMED(name, sub, root) \
for (XMLElem* sub = root->FirstChildElement(name); sub != nullptr; sub = sub->NextSiblingElement(name))

#define XML_ID(node)		node->Attribute("xml:id")
#define XML_WITH_ID(node)	node->Attribute("with_id")

#define XML_ASSERT_NODE_NAME(name, node)		if (std::string(name) != std::string(node->Name())) {throw Exception("expected " + std::string(name) + " got " + node->Name());}

#endif // XML_H