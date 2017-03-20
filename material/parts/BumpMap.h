#ifndef BUMPMAP_H
#define BUMPMAP_H


#include "Buildable.h"
#include "../ShaderVariable.h"

namespace MatPart {

	class BumpMap : public Buildable {

	public:

		virtual void build(Material2* material, XMLElem* elem) override {

			// vertex shader
			//material->getVertexParams().addVariable("normalRotMat", "out mat3 normalRotMat;");
			material->getVertexParams().addVariable(ShaderVariable(ShaderVariable::Mode::OUTBOUND, ShaderVariable::Type::MAT3, "normalRotMat"));


//			// get the matrix describing the bump-maps coordinate system (normal, tangent, bitangent)
//			material->getVertexParams().addFunction("getNormalRotationMatrix", R"(
//				mat3 getNormalRotationMatrix(const vec3 normal, const vec3 tangent) {
//					vec3 bitangent = cross(normal, tangent);
//					return mat3(tangent, bitangent, normal);
//				}
//			)", {}, {});

//			// the matrix to rotate a bump-map-normal into the vertex's normal
//			material->getVertexParams().addMain("\tnormalRotMat = getNormalRotationMatrix(" FRAG_NORMAL_WORLD_POS ", " FRAG_TANGENT_WORLD_POS ");\n");
//			material->getVertexParams().usedVariable("normalRotMat");

			// rotation matrix for bump-map-normals using mat3(tagent, bitangent, normal)
			//material->getVertexParams().addMainLine("\tnormalRotMat = mat3("VERT_VERTEX_TANGENT", cross("VERT_VERTEX_NORMAL", "VERT_VERTEX_TANGENT"), "VERT_VERTEX_NORMAL");");
			material->getVertexParams().addMainLine("normalRotMat = mat3(" FRAG_TANGENT_WORLD_POS ", normalize(cross(" FRAG_NORMAL_WORLD_POS ", " FRAG_TANGENT_WORLD_POS ")), " FRAG_NORMAL_WORLD_POS ");");

			// fragment shader
			material->getFragmentParams().addVariable(ShaderVariable(ShaderVariable::Mode::INBOUND, ShaderVariable::Type::MAT3, "normalRotMat"));
			//material->getFragmentParams().addVariable("normalRotMat", "in mat3 normalRotMat;");
			material->getFragmentParams().usedVariable("normalRotMat");

			XMLElem* eTex = elem->FirstChildElement("texture");
			XMLElem* eTexMix = elem->FirstChildElement("textureMix");

			if (eTex) {

				MatPart::SourceTexture mpTex;
				mpTex.build(material, eTex);
				material->getFragmentParams().addFunction("getBumpMapNormal",
				    "vec3 getBumpMapNormal() {return normalize(" + mpTex.get() + ".rgb - 0.5);}", {}, {}
				);

			} else if (eTexMix) {

				MatPart::SourceTextureMix mpTex;
				mpTex.build(material, eTexMix);
				material->getFragmentParams().addFunction("getBumpMapNormal",
				    "vec3 getBumpMapNormal() {return normalize(" + mpTex.get() + ".rgb - 0.5);}", {}, {}
				);

			}

			material->getFragmentParams().addFunction("getNormal", "vec3 getNormal() {return normalRotMat * getBumpMapNormal();}", {"normalRotMat"}, {"getBumpMapNormal"});

			material->getFragmentParams().usedVariable("normalRotMat");
			material->getVertexParams().usedVariable("normalRotMat");

			material->getFragmentParams().usedVariable(FRAG_NORMAL_WORLD_POS);
			material->getVertexParams().usedVariable(FRAG_NORMAL_WORLD_POS);
			material->getVertexParams().usedVariable(VERT_VERTEX_NORMAL);

			material->getFragmentParams().usedVariable(FRAG_TANGENT_WORLD_POS);
			material->getVertexParams().usedVariable(FRAG_TANGENT_WORLD_POS);
			material->getVertexParams().usedVariable(VERT_VERTEX_TANGENT);

		}

	};


}

#endif // BUMPMAP_H
