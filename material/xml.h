#ifndef HELPER_XML_H
#define HELPER_XML_H

#include "../lib/tinyxml2/tinyxml2.h"

using XMLDoc = tinyxml2::XMLDocument;
using XMLErr = tinyxml2::XMLError;
using XMLNode = tinyxml2::XMLNode;
using XMLElem = tinyxml2::XMLElement;
using XMLAttr = tinyxml2::XMLAttribute;
using XMLText = tinyxml2::XMLText;

#define XML_FOREACH_ATTR(attr, root) \
for (const XMLAttr* attr = root->FirstAttribute(); attr != nullptr; attr = attr->Next())

#define XML_FOREACH_NODE(sub, root) \
for (const XMLNode* sub = root->FirstChild(); sub != nullptr; sub = sub->NextSibling())

#define XML_FOREACH_ELEM(sub, root) \
for (XMLElem* sub = (XMLElem*)root->FirstChild(); sub != nullptr; sub = (XMLElem*)sub->NextSibling())

#define XML_FOREACH_ELEM_NAMED(name, sub, root) \
for (XMLElem* sub = root->FirstChildElement(name); sub != nullptr; sub = sub->NextSiblingElement(name))

#define XML_ID(node)		node->Attribute("xml:id")
#define XML_WITH_ID(node)	node->Attribute("with_id")

#define XML_ASSERT_NODE_NAME(name, node)		if (std::string(name) != std::string(node->Name())) {throw Exception("expected " + std::string(name) + " got " + node->Name());}

#define XML_MANDATORY_ATTR(node, attr)		(node->Attribute(attr) ? node->Attribute(attr) : throw Exception(std::string("missing XML attribute: ") + attr));


#endif // HELPER_XML_H
