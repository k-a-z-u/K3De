#ifndef MATERIALFACTORY3IMPL_H
#define MATERIALFACTORY3IMPL_H

#include "Material3.h"
#include "MaterialFactory3.h"

#include "../scene/ShadowRenderer.h"
#include "../scene/Scene.h"
#include "../textures/Texture2D.h"

static inline PXMLNode recurseNode(XMLElem* elem) {
	PXMLNode node(elem->Name());
	XML_FOREACH_ATTR(attr, elem) {
	//for (const XMLAttr* attr = elem->FirstAttribute(); attr != nullptr; attr = attr->Next()) {
		const std::string aName = attr->Name();
		const std::string aVal = attr->Value();
		node.attributes[aName] = aVal;
	}
	//for (const XMLNode* node = elem->FirstChild(); node != nullptr; node = node->NextSibling()) {
	XML_FOREACH_ELEM(sub, elem) {

		if (sub->ToText()) {
			continue;}	// skip text nodes
		node.children.push_back(recurseNode(sub));
	}
	return node;
}

void MaterialFactory3::loadUserValues(Material3* mat, XMLElem* elem) {

//	if (!elem) {return;}

//	// structured
//	XML_FOREACH_ELEM_NAMED("value", eVal, elem) {
//		const std::string id = XML_ID(eVal);
//		const std::string value = eVal->FirstChild()->Value();
//		mat->getUserValues()[id] = value;
//	}

//	// whole subtree
//	mat->userNode = recurseNode(elem);
//	(void) mat;

}

void Components::addDefaults() {
	uv["default"] = new ProvUVDefault();
	uv["reflect"] = new ProvUVReflect();
	uv["refract"] = new ProvUVRefract();
}

ProvUV* Components::getUV(const std::string& id) const {
	const auto& it = this->uv.find(id);
	if (it == this->uv.end()) {throw Exception("no such UV-provider: " + id);}
	return it->second;
}

ProvColor* Components::getColor(const std::string& id) const {
	const auto& it = this->color.find(id);
	if (it == this->color.end()) {throw Exception("no such color-provider: " + id);}
	return it->second;
}

void Components::parseUV(Material3* mat, XMLElem* elem) {

	XML_FOREACH_ELEM(sub, elem) {

		const std::string subName = sub->Name();
		const std::string id = XML_ID(sub);

		if ("time" == subName) {

			ProvUV* parent = getUV(sub->Attribute("src"));
			const float sx = sub->FloatAttribute("scaleX", 1);
			const float sy = sub->FloatAttribute("scaleY", 1);
			ProvUVModTime* prov = new ProvUVModTime(parent, ProvUVModTime::Mode::ADD, Vec2(sx, sy));
			uv[id] = prov;

		} else if ("dudv" == subName) {

			ProvUV* uvToMod = getUV(sub->Attribute("src"));
			ProvColor* dudvColor = getColor(sub->Attribute("dudv"));
			const float sx = sub->FloatAttribute("scaleX", 1);
			const float sy = sub->FloatAttribute("scaleY", 1);
			ProvUVModUVDisplace* prov = new ProvUVModUVDisplace(uvToMod, dudvColor, sx, sy);
			uv[id] = prov;

		}

	}

}

void Components::parseColor(Material3 *mat, XMLElem *elem) {

	XML_FOREACH_ELEM(sub, elem) {

		const std::string subName = sub->Name();
		const std::string id = XML_ID(sub);

		if ("texture" == subName) {

			const std::string texID = sub->Attribute("texture");
			const std::string uvID = sub->Attribute("uv");
			const float tx = sub->FloatAttribute("tileX", 1);
			const float ty = sub->FloatAttribute("tileY", 1);
			NamedTexture nt = mat->getTexture(texID);
			ProvUV* provUV = getUV(uvID);
			ProvTexture* tex = new ProvTexture(provUV, nt, Vec2(tx, ty));
			ProvColorTexture* prov = new ProvColorTexture(tex);
			color[id] = prov;

		} else if ("mix" == subName) {

			const std::string src1 = sub->Attribute("src1");
			const std::string src2 = sub->Attribute("src2");
			const float amount = sub->FloatAttribute("amount", 0.5);
			ProvColor* col1 = getColor(src1);
			ProvColor* col2 = getColor(src2);
			ProvColorMix* prov = new ProvColorMix(col1, col2, amount);
			color[id] = prov;

		} else if ("mixMask" == subName) {

			const std::string src1 = sub->Attribute("src1");
			const std::string src2 = sub->Attribute("src2");
			const std::string mask = sub->Attribute("mask");
			ProvColor* col1 = getColor(src1);
			ProvColor* col2 = getColor(src2);
			ProvColor* cmsk = getColor(mask);
			ProvColorMixMask* prov = new ProvColorMixMask(col1, col2, cmsk);
			color[id] = prov;

		} else if ("fixed" == subName) {

		} else if ("water" == subName) {

			const std::string uvReflect = sub->Attribute("uv_reflect");
			const std::string uvRefract = sub->Attribute("uv_refract");
			ProvUV* provUVreflect = getUV(uvReflect);
			ProvUV* provUVrefract = getUV(uvRefract);
			ProvUV* provUVdepth = getUV("default"); // TODO?!
			ProvColorWater* prov = new ProvColorWater(provUVreflect, provUVrefract, provUVdepth);
			color[id] = prov;

		}

	}

}


void MaterialFactory3::loadComponents(Material3* mat, XMLElem* elem) {

	if (!elem) {
		throw Exception("shader has no <components>!");
	}

	comp.addDefaults();

	XML_FOREACH_ELEM(sub, elem) {
		const std::string subName = sub->Name();
		if		("uv" == subName)		{comp.parseUV(mat, sub);}
		else if	("color" == subName)	{comp.parseColor(mat,sub);}
	}

}

void MaterialFactory3::loadTextures(Material3* mat, XMLElem* elem) {

	if (!elem) {return;}
	XML_FOREACH_ELEM_NAMED("texture", tex, elem) {

		const std::string id = XML_ID(tex);
		const std::string file = tex->FirstChildElement("file")->GetText();
		const bool compress = tex->BoolAttribute("compress", true);
		const bool mipMaps = tex->BoolAttribute("mipMaps", true);
//		const int idx = mat->getTextures().size();	// next free index

		Resource res(mat->dataPath + file);
		Texture2D* texture = scene->getTextureFactory().create(res, compress, mipMaps);
		mat->addTexture(id, texture);
//		//mat->paramsFragment.addVariable(id, "uniform sampler2D " + id + ";");
//		mat->paramsFragment.addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::TEXTURE_2D, id));

	}

}

//static ProvUV* buildUV(Material3* mat, XMLElem* elem) {
//	ProvUVDefault* provUV = new ProvUVDefault();
//	return provUV;
//}

void MaterialFactory3::buildDiffuse(Material3* mat, XMLElem* elem) {

	const std::string colorID = elem->Attribute("src");
	mat->setColor(comp.getColor(colorID));

//	if (subName == "texture") {
//		ProvUV* provUV = buildUV(mat, sub);
//		const std::string texID = sub->Attribute("with_id");
//		NamedTexture nt = mat->getTexture(texID);
//		ProvTexture* tex = new ProvTexture(provUV, nt);
//		ProvColorTexture* prov = new ProvColorTexture(tex);
//		mat->setColor(prov);
//	} else if (subName == "fixed") {
//		Vec4 color( sub->FloatAttribute("r"), sub->FloatAttribute("g"), sub->FloatAttribute("b"), sub->FloatAttribute("a") );
//		ProvColorFixed* prov = new ProvColorFixed(color);
//		mat->setColor(prov);
//	} else if (subName == "water") {
//		const std::string uvReflect = sub->Attribute("uv_reflect");
//		const std::string uvRefract = sub->Attribute("uv_refract");
//		ProvUV* provUVreflect = getUV(uvReflect);
//		ProvUV* provUVrefract = getUV(uvRefract);
//		ProvColorWater* prov = new ProvColorWater(provUVreflect, provUVrefract);
//		mat->setColor(prov);
//	} else {
//		throw Exception("todo");
//		// TODO?? fixed color?
//	}

}

//ProvTexture* MaterialFactory3::getTexture(Material3* mat, XMLElem* elem) {
//	const std::string texID = elem->Attribute("with_id");
//	const std::string uv = elem->Attribute("uv");
//	const float tileX = elem->FloatAttribute("tileX", 1);
//	const float tileY = elem->FloatAttribute("tileY", 1);
//	ProvUV* provUV = getUV(uv);
//	NamedTexture nt = mat->getTexture(texID);
//	return new ProvTexture(provUV, nt, Vec2(tileX, tileY));
//}

void MaterialFactory3::buildBumpMap(Material3* mat, XMLElem* elem) {

	const std::string colorID = elem->Attribute("src");
	ProvNormal* def = new ProvNormalDefault();
	ProvNormal* prov = new ProvBumpMapTexture(def, comp.getColor(colorID));
	mat->setNormal(prov);

	// TODO
//	const std::string colorID = elem->Attribute("color");
//	mat->setColor(comp.getColor(colorID));
	return;

//	XMLElem* sub = elem->FirstChildElement();
//	const std::string subName = sub->Name();

//	if (subName == "texture") {
//		ProvNormal* provNormal = mat->getNormal();
//		ProvTexture* tex = getTexture(mat, sub);
//		ProvBumpMapTexture* prov = new ProvBumpMapTexture(provNormal, tex);
//		mat->setNormal(prov);
//	} else {
//		throw Exception("todo");
//	}

}

static void buildSpecular(Material3* mat, XMLElem* elem) {


	XMLElem* sub = elem->FirstChildElement();
	const std::string subName = sub->Name();

	if (subName == "fixed") {
		const float shininess = sub->FloatAttribute("shininess");
		const float mul = sub->FloatAttribute("multiplier");
		const Vec3 color = Vec3(1,1,1) * mul;
		ProvSpecularFixed* provSpec = new ProvSpecularFixed(color, shininess);
		((ProvLightedPhong*)mat->getLighting())->setSpecular(provSpec);
	} else {
		throw Exception("todo");
	}

}

inline void assertAttr(XMLElem* elem, const char* attrName, const std::string& err) {
	if (!elem->Attribute(attrName)) {
		throw Exception(err);
	}
}

void MaterialFactory3::loadConfig(Material3* mat, XMLElem* elem) {

	// start with the default normal
	ProvNormalDefault* normalDef = new ProvNormalDefault();
	mat->setNormal(normalDef);

	if (!elem) {return;}
	XML_FOREACH_ELEM(sub, elem) {

		const std::string subName = sub->Name();

		if (subName == "usesTextures") {
			int i = 0; (void) i;
		} else if (subName == "lighting") {
			const std::string model = sub->Attribute("model");
			if		("none" == model)	{mat->setLighting(new ProvLightedNone());}
			else if	("phong" == model)	{mat->setLighting(new ProvLightedPhong());}
		} else if (subName == "shadowing") {
			assertAttr(sub, "receive", "<shadowing> needs receive='true/false' attribute");
			assertAttr(sub, "cast", "<shadowing> needs cast='true/false' attribute");
			mat->setReceivesShadows(sub->BoolAttribute("receive", false));
			mat->setCastsShadows(sub->BoolAttribute("cast", false));
			if (mat->getReceivesShadows()) {
				assertAttr(sub, "quality", "<shadowing> needs quality='f', f[0.0:1.0] attribute");
				const float quality = sub->FloatAttribute("quality");
				mat->getLighting()->setShadowing(new ProvShadow(quality));
			}
		} else if (subName == "diffuse") {
			buildDiffuse(mat, sub);
		} else if (subName == "bumpMap") {
			buildBumpMap(mat, sub);
		} else if (subName == "specular") {
			buildSpecular(mat, sub);
		} else if (subName == "usesClipping") {
			mat->setClipping(new ProvClippingZ());
		}

	}

}

#include "Material3Compiler.h"

void MaterialFactory3::check(Material3* mat) {

	if (!mat->color) {throw Exception("material needs a color provider (fixed, texture, vertex, ..");}
	if (!mat->lighting) {throw Exception("material needs a lighting provider that determines the shading");}

}

void MaterialFactory3::compile(Material3* mat) {

	Material3Compiler comp(mat);
	comp.compileShader(this->scene);

}

//ProvUV* MaterialFactory3::getUV(const std::string& name) {
//	const auto& it = this->uv.find(name);
//	if (it == this->uv.end()) {
//		throw Exception("no such UV-provider: " + name);
//	}
//	return it->second;
//}

//void MaterialFactory3::loadModifiers(Material3* mat, XMLElem* elem) {


//	if (!elem) {return;}
//	XML_FOREACH_ELEM(sub, elem) {

//		const std::string subName = sub->Name();

//		if (subName == "uv") {
//			loadModifiersUV(mat, sub);
//		} else if (subName == "color") {
//			throw Exception("TODO");
//		} else {
//			throw Exception("invalid modifier group: " + subName);
//		}

//	}

//}

void MaterialFactory3::loadModifiersUV(Material3* mat, XMLElem* elem) {

	if (!elem) {return;}
	XML_FOREACH_ELEM(sub, elem) {

		const std::string subName = sub->Name();
		const std::string id = sub->Attribute("xml:id");

		if (subName == "time") {
//			ProvUV* parent = getUV(sub->Attribute("src"));
//			const float sx = sub->FloatAttribute("scaleX", 1);
//			const float sy = sub->FloatAttribute("scaleY", 1);
//			ProvUVModTime* prov = new ProvUVModTime(parent, ProvUVModTime::Mode::ADD, Vec2(sx, sy));
//			uv[id] = prov;
		} else if (subName == "dudv") {

//			XMLElem* tex = sub->FirstChildElement("texture");
//			ProvUV* toMod = getUV(sub->Attribute("src"));
//			//ProvUV* forTex = getUV(tex->Attribute("src"));
//			//NamedTexture nt = mat->getTexture(tex->Attribute("with_id"));	// TODO inflexible. tiling? ..
//			//ProvTexture* provTex = new ProvTexture(forTex, nt);
//			ProvTexture* provTex = getTexture(mat, tex);

//			const float sx = sub->FloatAttribute("scaleX", 1);
//			const float sy = sub->FloatAttribute("scaleY", 1);
//			ProvUVModUVDisplace* prov = new ProvUVModUVDisplace(toMod, provTex, sx, sy);
//			uv[id] = prov;

		}

	}

}

//void MaterialFactory3::buildLighting(Material3 *mat, XMLElem *cfg) {

//	XMLElem* elem = cfg->FirstChildElement("usesLighting");
//	if (elem) {
//		mat->lighting = new MatPart::Lighting();
//		mat->lighting->setEnabled(true);
//		mat->lighting->build(mat, elem);
//	} else {
//		mat->lighting = new MatPart::Lighting();
//		mat->lighting->setEnabled(false);
//	}

//}

//void MaterialFactory3::buildDiffuse(Material3* mat, XMLElem* cfg) {

//	XMLElem* elem = cfg->FirstChildElement("diffuse");
//	if (elem) {

//		XMLElem* eTex = elem->FirstChildElement("texture");
//		XMLElem* eTexMix = elem->FirstChildElement("textureMix");
//		XMLElem* eCol = elem->FirstChildElement("color");
//		XMLElem* eWater = elem->FirstChildElement("water");

//		// diffuse texture
//		if (eTex) {

//			MatPart::SourceTexture mpTex;
//			mpTex.build(mat, eTex);

//			mat->paramsFragment.addFunction("getDiffuseColor", "vec4 getDiffuseColor() {return " + mpTex.get() + ";}", {}, {});

//		// multiple textures, blended together
//		} else if (eTexMix) {

//			MatPart::SourceTextureMix mpTex;
//			mpTex.build(mat, eTexMix);

//			mat->paramsFragment.addFunction("getDiffuseColor", "vec4 getDiffuseColor() {return " + mpTex.get() + ";}", {}, {});

//		// fixed diffuse color
//		} else if (eCol) {

//			MatPart::SourceColor mpCol;
//			mpCol.build(mat, eCol);

//			mat->paramsFragment.addFunction("getDiffuseColor", "vec4 getDiffuseColor() {return " + mpCol.get() + ";}", {}, {});

//		// water surface?
//		} else if (eWater) {

//			//mat->paramsFragment.addVariable("texReflect", "uniform sampler2D texReflect;");
//			//mat->paramsFragment.addVariable("texRefract", "uniform sampler2D texRefract;");

//			mat->paramsFragment.addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::TEXTURE_2D, "texReflect"));
//			mat->paramsFragment.addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::TEXTURE_2D, "texRefract"));

//			MatPart::SourceTexture texReflect("texReflect", scene->getWaterRenderer()->getReflect());
//			MatPart::SourceTexture texRefract("texRefract", scene->getWaterRenderer()->getRefract());

//			texReflect.buildModifiers(mat, eWater);
//			texRefract.buildModifiers(mat, eWater);

//			mat->paramsFragment.addFunction("getDiffuseColor", std::string() +
//				"vec4 getDiffuseColor() {\n" +

//				// texture coordinates
//			    "	vec2 uvRefract = (" + FRAG_VERTEX_SCREEN_POS + ".xy / " + FRAG_VERTEX_SCREEN_POS + ".w) * 0.5 + vec2(0.5, 0.5);\n" +
//				"	vec2 uvReflect = vec2(uvRefract.x, 1.0-uvRefract.y);\n" +

//				// texture values
//			    "	vec4 cReflect = " + texReflect.get("uvReflect") + ";\n" +
//			    "	vec4 cRefract = " + texRefract.get("uvRefract") + ";\n" +

//				// texture mix
//				"	vec3 N = vec3(0,1,0);\n" +
//				"	vec3 E = normalize(surfaceToEye_M);\n" +
//				"	float waterTexMix = clamp(pow(dot(N,E), 2.0), 0.1, 0.9);\n" +
//				"	return mix(cReflect, cRefract, waterTexMix);\n" +
//				"}", {FRAG_VERTEX_SCREEN_POS, "surfaceToEye_M", "texReflect", "texRefract"}, {}
//			);

//TODO("better solution??")
//			mat->paramsVertex.usedVariable(FRAG_VERTEX_SCREEN_POS);
//			mat->paramsVertex.addMainLine("vertex_S = gl_Position;");

//		}

//TODO("todo: if only for materials with alpha")
//TODO("add alpha/blending support to the materials def")
//		// early opt-out for almost transparent pixels
//		mat->paramsFragment.addMainLine(ShaderVariable(ShaderVariable::Type::COLOR4, "diffuseColor").getDecl() + " = getDiffuseColor();");
//		mat->paramsFragment.addMainLine("if (diffuseColor.a < 0.01) {discard;}");
//		mat->paramsFragment.addMainLine("");

//		mat->paramsFragment.usedFunction("getDiffuseColor");

//	} else {
//		throw Exception("missing <diffuse>");
//	}

//}

//void MaterialFactory3::buildShadow(Material3 *mat, XMLElem* cfg) {

//	if (cfg->FirstChildElement("castsShadows")) {
//		mat->castsShadows = true;
//	} else {
//		mat->castsShadows = false;
//	}

//	XMLElem* elem = cfg->FirstChildElement("receiveShadows");
//	if (elem) {

//		mat->receivesShadows = true;
//		MatPart::Shadowing shadowing;
//		shadowing.build(mat, elem);

//	}

//}

//void MaterialFactory3::buildBumpMap(Material3* mat, XMLElem* cfg) {

//	XMLElem* elem = cfg->FirstChildElement("bumpMap");
//	if (elem) {

//		MatPart::BumpMap bm;
//		bm.build(mat, elem);

//	} else {

//TODO("only if lighting is used at all!")
//		mat->paramsFragment.addFunction("getNormal", "vec3 getNormal() {return normalize(" FRAG_NORMAL_WORLD_POS ");}", {FRAG_NORMAL_WORLD_POS}, {});
//		mat->paramsFragment.usedVariable(FRAG_NORMAL_WORLD_POS);
//		mat->paramsVertex.usedVariable(FRAG_NORMAL_WORLD_POS);

//	}

//	// normal within the main code
//	mat->paramsFragment.addMainLine(ShaderVariable(ShaderVariable::Type::POS3, "N").getDecl() + " = getNormal();", 1);

//}

//void MaterialFactory3::setClipping(Material3* mat, XMLElem* cfg) {

//	XMLElem* elem = cfg->FirstChildElement("usesClipping");
//	if (elem) {
//		mat->useClipping = true;
//		MatPart::Clipping clip;
//		clip.build(mat, elem);
//		mat->paramsFragment.addMainLine(clip.doClip(mat), 1);
//	} else {
//		mat->useClipping = false;
//	}

//}


//void MaterialFactory3::compile(Material3 *mat) {


//}


#endif // MATERIALFACTORY3IMPL_H
