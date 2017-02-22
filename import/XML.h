#ifndef IMPORT_XML_H
#define IMPORT_XML_H

#include "../material/xml.h"
#include "../data/ResourceFactory.h"
#include <memory>

class XML {

public:

	static std::shared_ptr<XMLDoc> get(const Resource& res) {

		const Data data = ResourceFactory::get().get(res);

		std::shared_ptr<XMLDoc> doc = std::make_shared<XMLDoc>();
		assertOK(doc->Parse((const char*)data.get(), data.size()), *doc, "error while parsing XML");

		return doc;

	}

private:

	static void assertOK(XMLErr res, XMLDoc& doc, const std::string& txt) {
		if (res != tinyxml2::XMLError::XML_SUCCESS) {
			const std::string err = doc.ErrorName();
			const std::string add = doc.GetErrorStr1();
			throw Exception(txt + ": " + err + " - " + add);
		}
	}

};

#endif // IMPORT_XML_H
