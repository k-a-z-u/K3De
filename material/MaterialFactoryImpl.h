#ifndef MATERIALFACTORYIMPL_H
#define MATERIALFACTORYIMPL_H

#include "MaterialFactory.h"
#include "parts/modifiers/ModifierFactory.h"
#include "parts/sources/SourceTexture.h"
#include "parts/sources/SourceColor.h"
#include "parts/BumpMap.h"
#include "parts/Shadowing.h"
#include "parts/Specular.h"
#include "parts/Lighting.h"
#include "parts/Clipping.h"

#include "../scene/ShadowRenderer.h"
#include "../scene/Scene.h"
#include "../textures/Texture2D.h"

static inline PXMLNode recurse(XMLElem* elem) {
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
		node.children.push_back(recurse(sub));
	}
	return node;
}

void MaterialFactory::loadUserValues(Material2* mat, XMLElem* elem) {

	if (!elem) {return;}

	// structured
	XML_FOREACH_ELEM_NAMED("value", eVal, elem) {
		const std::string id = XML_ID(eVal);
		const std::string value = eVal->FirstChild()->Value();
		mat->getUserValues()[id] = value;
	}

	// whole subtree
	mat->userNode = recurse(elem);
	(void) mat;

}

void MaterialFactory::loadTextures(Material2* mat, XMLElem* elem) {

	if (!elem) {return;}
	XML_FOREACH_ELEM_NAMED("texture", tex, elem) {

		const std::string id = XML_ID(tex);
		const std::string file = tex->FirstChildElement("file")->GetText();
		const bool compress = tex->BoolAttribute("compress", true);
		const bool mipMaps = tex->BoolAttribute("mipMaps", true);
		const int idx = mat->getTextures().size();	// next free index

		//Texture2D* texture = scene->getTextureFactory().create(mat->dataPath + file, compress, mipMaps);

		Resource res(mat->dataPath + file);
		Texture2D* texture = scene->getTextureFactory().create(res, compress, mipMaps);
		MatPart::LoadedTexture t(id, idx, texture);
		mat->getTextures().add(t);
		//mat->paramsFragment.addVariable(id, "uniform sampler2D " + id + ";");
		mat->paramsFragment.addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::TEXTURE_2D, id));

	}

}

void MaterialFactory::loadModifiers(Material2* mat, XMLElem* elem) {
	if (!elem) {return;}
	XML_FOREACH_ELEM(node, elem) {
		MatPart::Modifier* mod = MatPart::ModifierFactory::get(mat, node);
		_assertNotNull(mod, "coding error. modifier is null?!");
		mat->modifiers.add(mod);
		int i = 0; (void) i;
	}
}

void MaterialFactory::buildLighting(Material2 *mat, XMLElem *cfg) {

	XMLElem* elem = cfg->FirstChildElement("usesLighting");
	if (elem) {

		mat->useLigthing = true;
		MatPart::Lighting l;
		l.build(mat, cfg);

	}

}

void MaterialFactory::buildDiffuse(Material2* mat, XMLElem* cfg) {

	XMLElem* elem = cfg->FirstChildElement("diffuse");
	if (elem) {

		XMLElem* eTex = elem->FirstChildElement("texture");
		XMLElem* eCol = elem->FirstChildElement("color");
		XMLElem* eWater = elem->FirstChildElement("water");

		// diffuse texture
		if (eTex) {

			MatPart::SourceTexture mpTex;
			mpTex.build(mat, eTex);

			mat->paramsFragment.addFunction("getDiffuseColor", "vec4 getDiffuseColor() {return " + mpTex.get() + ";}", {}, {});

		// fixed diffuse color
		} else if (eCol) {

			MatPart::SourceColor mpCol;
			mpCol.build(mat, eCol);

			mat->paramsFragment.addFunction("getDiffuseColor", "vec4 getDiffuseColor() {return " + mpCol.get() + ";}", {}, {});

		// water surface?
		} else if (eWater) {

			//mat->paramsFragment.addVariable("texReflect", "uniform sampler2D texReflect;");
			//mat->paramsFragment.addVariable("texRefract", "uniform sampler2D texRefract;");

			mat->paramsFragment.addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::TEXTURE_2D, "texReflect"));
			mat->paramsFragment.addVariable(ShaderVariable(ShaderVariable::Mode::UNIFORM, ShaderVariable::Type::TEXTURE_2D, "texRefract"));

			MatPart::SourceTexture texReflect("texReflect", scene->getWaterRenderer()->getReflect());
			MatPart::SourceTexture texRefract("texRefract", scene->getWaterRenderer()->getRefract());

			texReflect.buildModifiers(mat, eWater);
			texRefract.buildModifiers(mat, eWater);

			mat->paramsFragment.addFunction("getDiffuseColor", std::string() +
				"vec4 getDiffuseColor() {\n" +

				// texture coordinates
			    "	vec2 uvRefract = (" + FRAG_VERTEX_SCREEN_POS + ".xy / " + FRAG_VERTEX_SCREEN_POS + ".w) * 0.5 + vec2(0.5, 0.5);\n" +
				"	vec2 uvReflect = vec2(uvRefract.x, 1.0-uvRefract.y);\n" +

				// texture values
			    "	vec4 cReflect = " + texReflect.get("uvReflect") + ";\n" +
			    "	vec4 cRefract = " + texRefract.get("uvRefract") + ";\n" +

				// texture mix
				"	vec3 N = vec3(0,1,0);\n" +
				"	vec3 E = normalize(surfaceToEye_M);\n" +
				"	float waterTexMix = pow(dot(N,E), 2.0);\n" +
				"	return mix(cReflect, cRefract, waterTexMix);\n" +
				"}", {FRAG_VERTEX_SCREEN_POS, "surfaceToEye_M", "texReflect", "texRefract"}, {}
			);

TODO("better solution??")
			mat->paramsVertex.usedVariable(FRAG_VERTEX_SCREEN_POS);
			mat->paramsVertex.addMainLine("vertex_S = gl_Position;");

		}

TODO("todo: if only for materials with alpha")
TODO("add alpha/blending support to the materials def")
		// early opt-out for almost transparent pixels
		mat->paramsFragment.addMainLine(ShaderVariable(ShaderVariable::Type::COLOR4, "diffuseColor").getDecl() + " = getDiffuseColor();");
		mat->paramsFragment.addMainLine("if (diffuseColor.a < 0.01) {discard;}");
		mat->paramsFragment.addMainLine("");

		mat->paramsFragment.usedFunction("getDiffuseColor");

	} else {
		throw Exception("missing <diffuse>");
	}

}

void MaterialFactory::buildShadow(Material2 *mat, XMLElem* cfg) {

	if (cfg->FirstChildElement("castsShadows")) {
		mat->castsShadows = true;
	} else {
		mat->castsShadows = false;
	}

	XMLElem* elem = cfg->FirstChildElement("receiveShadows");
	if (elem) {

		mat->receivesShadows = true;
		MatPart::Shadowing shadowing;
		shadowing.build(mat, elem);

	}

}

void MaterialFactory::buildBumpMap(Material2* mat, XMLElem* cfg) {

	XMLElem* elem = cfg->FirstChildElement("bumpMap");
	if (elem) {

		MatPart::BumpMap bm;
		bm.build(mat, elem);

	} else {

TODO("only if lighting is used at all!")
		mat->paramsFragment.addFunction("getNormal", "vec3 getNormal() {return normalize(" FRAG_NORMAL_WORLD_POS ");}", {FRAG_NORMAL_WORLD_POS}, {});
		mat->paramsFragment.usedVariable(FRAG_NORMAL_WORLD_POS);
		mat->paramsVertex.usedVariable(FRAG_NORMAL_WORLD_POS);

	}

	// normal within the main code
	mat->paramsFragment.addMainLine(ShaderVariable(ShaderVariable::Type::POS3, "N").getDecl() + " = getNormal();", 1);

}

void MaterialFactory::setClipping(Material2* mat, XMLElem* cfg) {

	XMLElem* elem = cfg->FirstChildElement("usesClipping");
	if (elem) {
		mat->useClipping = true;
		MatPart::Clipping clip;
		clip.build(mat, elem);
		mat->paramsFragment.addMainLine(clip.doClip(mat), 1);
	} else {
		mat->useClipping = false;
	}

}

void MaterialFactory::buildSpecular(Material2* mat, XMLElem* cfg) {

	XMLElem* elem = cfg->FirstChildElement("specular");
	if (elem) {

		mat->useSpecular = true;

		MatPart::Specular spec;
		spec.build(mat, elem);


//		XMLElem* eFixed = elem->FirstChildElement("fixed");
//		if (eFixed) {

//			const float shininess = eFixed->FloatAttribute("shininess");
//			const float multiplier = eFixed->FloatAttribute("multiplier");

//			//mat->paramsFragment.addMainLine("\tvec3 specularColor = ambient.rgb * " + std::to_string(multiplier) + "f;");
//			mat->paramsFragment.addMainLine("\tvec3 specularColor = vec3(1.0) * " + std::to_string(multiplier) + "f;");
//			mat->paramsFragment.addMainLine("\tconst float specularShininess = " + std::to_string(shininess) + "f;");

//		}

	} else {

		mat->useSpecular = false;

	}

}

void MaterialFactory::buildPixel(Material2 *mat) {

	// ligth calculation
	// final = emission   + ambient_L + diffuse_L + specular_L


	if (mat->usesLighting() && mat->useSpecular) {

		TODO("move to Lighting.h")

		// surface to camera [unit vector] for specular lighting
		mat->paramsFragment.addMainLine("vec3 E = normalize(" FRAG_SURFACE_TO_EYE ");");
		mat->paramsFragment.usedVariable(FRAG_SURFACE_TO_EYE);
		mat->paramsVertex.usedVariable(FRAG_SURFACE_TO_EYE);

	}


//	// the diffuse color from texture or fixed color that is affected by the lights
//	mat->paramsFragment.addMainLine("vec4 diffuseColor = getDiffuseColor();");
//	mat->paramsFragment.addMainLine("");


	if (!mat->usesLighting()) {

		// no lighting? keep color "as-is"
		mat->paramsFragment.addMainLine("color = diffuseColor;");

	} else {

		// start with an empty output color. keep alpha as-is
		mat->paramsFragment.addMainLine("color = vec4(0, 0, 0, diffuseColor.a);");

		// TODO
		// add emission [self illumination]
		if (1 == 0) {
			mat->paramsFragment.addMain("// EMISSION");
			mat->paramsFragment.addMainLine("color.rgb += diffuseColor * selfIllumFactor;");
		}

		// for (int i = 0; i < scene->getNumLights(); ++i) {

		MatPart::Lighting lighting;

		mat->paramsFragment.usedVariable("lights");

		// add processing for each light
		for (int i = 0; i < MAX_LIGHTS; ++i) {

			const std::string si = std::to_string(i);

			mat->paramsVertex.addMainSection("light " + si);

			mat->paramsVertex.addMainLine("if (" + lighting.isEnabled(si) + ") {"); {

				mat->paramsVertex.addMainLine(std::string() + FRAG_SHADOW_MAP_COORD + "[" + si + "] = lights.light["+si+"].pvShadow * vec4(vertex_M, 1.0);", 2);
				mat->paramsVertex.usedVariable(FRAG_SHADOW_MAP_COORD);

			} mat->paramsVertex.addMainLine("}");






			mat->paramsFragment.addMainSection("light " + si);

			// check whether the light is enabled
			mat->paramsFragment.addMainLine("if (" + lighting.isEnabled(si) + ") {"); {

				// calculate variable light stuff (different for every light)
				//mat->paramsFragment.addMainLine(ShaderVariable(ShaderVariable::Type::POS3, "lightPos_M").getDecl() + " = " + lighting.getPos(si) + ";", 2);	//getLightPos(" + si + ");", 2);

				// attenuation over distance
				//mat->paramsFragment.addMainLine("float lightAttenLinear = " + lighting.getAttenuationLinear(si) + ";" , 2);//getLightAttenuation(" + si + ");", 2);
				//mat->paramsFragment.addMainLine("float lightAttenQuad = " + lighting.getAttenuationQuadratic(si) + ";" , 2);//getLightAttenuation(" + si + ");", 2);
				mat->paramsFragment.addMainLine("lowp float distToLight = length(" + lighting.getPos(si) + " - vertex_M);", 2);

				// combine both, attenuation over distance and user-defined overall impact [usually 1.0]
				//mat->paramsFragment.addMainLine("float lightStrength = " + lighting.getImpact(si) + " / (1.0 + (lightAttenLinear*distToLight) + (lightAttenQuad*distToLight*distToLight));", 2);
				mat->paramsFragment.addMainLine("lowp float lightStrength = " + lighting.getImpact(si) + " / ", 2);
					mat->paramsFragment.addMainLine("(1.0f + (" + lighting.getAttenuationLinear(si) + "*distToLight) + ", 3);
					mat->paramsFragment.addMainLine("(" + lighting.getAttenuationQuadratic(si) + "*distToLight*distToLight));", 3);

				#define MIN_LIGHT_STRENGTH		"0.002"

				// also add [costly] shadow to strenght?
				if (mat->receivesShadows) {

					mat->paramsFragment.addMainSection("shadowing", 2);
					mat->paramsFragment.addMainLine(
						"if (lightStrength > " MIN_LIGHT_STRENGTH " && " + lighting.castsShadows(si) + ") {"
							"lightStrength *= getShadowAmount(texShadowMap["+si+"], "+si+");"						// faster or slower???
							//"lightStrength *= getShadowAmount(texShadowMap["+si+"], shadowMapCoord["+si+"]);"		// faster or slower???

						"}", 2
					);

				}

				// if the remaining strength is too low, just skip this light for performance reasons
				mat->paramsFragment.addMainSection("light still visible?", 2);
				mat->paramsFragment.addMainLine("if (lightStrength > " MIN_LIGHT_STRENGTH ") {", 2); {

					// light's diffuse color
					mat->paramsFragment.addMainSection("diffuse color", 3);

					//mat->paramsFragment.addMainLine(ShaderVariable(ShaderVariable::Type::COLOR3, "lightColor").getDecl() + " = " + lighting.getColor(si) + ";", 3);

					// from surface towards the light [unit-vector]
					mat->paramsFragment.addMainLine("vec3 L = normalize(" + lighting.getPos(si) + " - vertex_M);", 3);
					mat->paramsFragment.usedVariable("vertex_M");

					// diffuse angle
					mat->paramsFragment.addMainLine("lowp float theta = max( 0.0f, dot(N, L) );", 3);

					// add diffuse color
					mat->paramsFragment.addMainLine("color.rgb += diffuseColor.rgb * " + lighting.getColor(si) + " * lightStrength * theta;", 3);

					// use specular lighting?
					if (mat->useSpecular) {

						mat->paramsFragment.addMainSection("specular", 3);

						// specular angle
						mat->paramsFragment.addMainLine("vec3 R = reflect(-L, N);", 3);
						mat->paramsFragment.addMainLine("lowp float cosAlpha = max( 0.0f, dot(E, R) );", 3);

						// add specular color
						mat->paramsFragment.addMainLine("color.rgb += specularColor * pow(cosAlpha, specularShininess) * lightStrength;", 3);

					}

				} mat->paramsFragment.addMainLine("}", 2);

			} mat->paramsFragment.addMainLine("}");

		}

	}

}

void MaterialFactory::compile(Material2 *mat) {

	/** always needed: the projection of input vertex */
	mat->getVertexParams().addMainPre("\tgl_Position = PVM * vec4(vertex, 1.0);\n");
	mat->getVertexParams().usedVariable("vertex");
	mat->getVertexParams().usedVariable("PVM");

	/** the vertex in wold-space */
	mat->getVertexParams().addMainPre("\tvertex_M = (M * vec4(vertex, 1.0)).xyz;\n");
	mat->getVertexParams().usedVariable(FRAG_VERTEX_WORLD_POS);
	mat->getVertexParams().usedVariable("vertex");
	mat->getVertexParams().usedVariable("M");


	/** vertex-color used? -> add to vertex shader */
	if (mat->getFragmentParams().hasUsedVariable(FRAG_VERTEX_COLOR)) {
		mat->getVertexParams().addMainPre("\t" FRAG_VERTEX_COLOR " = color;\n");
		mat->getVertexParams().usedVariable("color");
	}

	/** texture coordinates used? -> add to vertex shader */
	if (mat->getFragmentParams().hasUsedVariable(FRAG_VERTEX_UV)) {
		mat->getVertexParams().addMainPre("\t" FRAG_VERTEX_UV " = texCoord;\n");
		mat->getVertexParams().usedVariable(FRAG_VERTEX_UV);
		mat->getVertexParams().usedVariable("texCoord");
	}

	/** the vertex's normal in world-space */
	if (mat->getFragmentParams().hasUsedVariable(FRAG_NORMAL_WORLD_POS)) {
		mat->getVertexParams().addMainPre("\t" FRAG_NORMAL_WORLD_POS " = normalize(mat3(M) * normal);\n");		// normals are NOT translated (only scaled and rotated) -> 3x3 matrix suffices
		mat->getVertexParams().usedVariable("normal");
		mat->getVertexParams().usedVariable("M");
	}

	/** the vertex's tangent in world-space */
	if (mat->getFragmentParams().hasUsedVariable(FRAG_TANGENT_WORLD_POS)) {
		mat->getVertexParams().addMainPre("\t" FRAG_TANGENT_WORLD_POS " = normalize(mat3(M) * tangent);\n");		// tangents are NOT translated (only scaled and rotated) -> 3x3 matrix suffices
		mat->getVertexParams().usedVariable("tangent");
		mat->getVertexParams().usedVariable("M");
	}

	/** the surface_to_camera in world-space */
	if (mat->getFragmentParams().hasUsedVariable(FRAG_SURFACE_TO_EYE)) {
		mat->getVertexParams().addMainPre("\t" FRAG_SURFACE_TO_EYE " = normalize(camPos - vertex_M);\n");
		mat->getVertexParams().usedVariable("vertex_M");
		mat->getVertexParams().usedVariable("camPos");
	}

	const std::string cVert = mat->getVertexParams().getCode();
	const std::string cFrag = mat->getFragmentParams().getCode();

	mat->shader = scene->getShaderFactory().createFromCode(cVert, cFrag);
	mat->shader->bind();

	const std::vector<MatPart::LoadedTexture>& textures = mat->getTextures().all();
	for (const MatPart::LoadedTexture& texture : textures) {
		if (mat->shader->hasUniform(texture.id)) {
			mat->shader->setInt(texture.id, texture.idx);
			mat->bindTextures.set(texture.idx, texture.tex);
		}
	}

TODO("TODO: assign fixed texture units for the following textures? less binding/unbinding")

	// additionally bind the shadow textures?
	if (mat->getReceivesShadows() && scene->getShadowRenderer()) {
		for (int i = 0; i < MAX_LIGHTS; ++i) {
			const std::string name = "texShadowMap[" + std::to_string(i) + "]";
			if (mat->shader->hasUniform(name)) {
				mat->shader->setInt(name, mat->bindTextures.size());
				mat->bindTextures.add(scene->getShadowRenderer()->getShadowTexture(i));
			}
		}
	}

	// additionally bind the water textures? [reflect/refract]
	if (scene->getWaterRenderer()) {
		if (mat->shader->hasUniform("texReflect")) {
			mat->shader->setInt("texReflect", mat->bindTextures.size());
			mat->bindTextures.add(scene->getWaterRenderer()->getReflect());
		}
		if (mat->shader->hasUniform("texRefract")) {
			mat->shader->setInt("texRefract", mat->bindTextures.size());
			mat->bindTextures.add(scene->getWaterRenderer()->getRefract());
		}
	}

	// done
	mat->shader->unbind();

}

#endif // MATERIALFACTORYIMPL_H
