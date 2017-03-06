#ifndef SOURCETEXTURE_H
#define SOURCETEXTURE_H

#include "Source.h"

namespace MatPart {

	class SourceTexture : public Source {

	private:

		std::string texID;
		std::string mode;
		Vec2 tile = Vec2(1,1);

		ITexture* tex;

		std::string r,g,b,a;


	public:

		SourceTexture() {
			;
		}

		SourceTexture(const std::string& texID, ITexture* tex) : texID(texID), tex(tex) {
			;
		}

		void build(Material2* material, XMLElem* node) override {

			// sanity check
			XML_ASSERT_NODE_NAME("texture", node);

			// fetch the (previously loaded) texture
			texID = node->Attribute("with_id");
			const MatPart::LoadedTexture& tex = material->getTextures().byID(texID);
			this->tex = tex.tex;

			material->getVertexParams().usedVariable(FRAG_VERTEX_UV);	// I need UV coordinates
			material->getFragmentParams().usedVariable(FRAG_VERTEX_UV);	// I need UV coordinates
			material->getFragmentParams().usedVariable(tex.id);	// I need the sampler variable

			// load additional modifiers [if any]
			buildModifiers(material, node);

			// parameters
			tile.x = node->FloatAttribute("tileX"); if (tile.x == 0) {tile.x = 1;}
			tile.y = node->FloatAttribute("tileY"); if (tile.y == 0) {tile.y = 1;}

			// mode
			mode = (node->Attribute("mode")) ? (node->Attribute("mode")) : ("rgba");

			// for mode=map
			if ("map" == mode) {
				r = XML_MANDATORY_ATTR(node, "r");
				g = XML_MANDATORY_ATTR(node, "g");
				b = XML_MANDATORY_ATTR(node, "b");
				a = XML_MANDATORY_ATTR(node, "a");
			}

		}

		/** get the underlying texture */
		ITexture* getTexture() const {
			return tex;
		}

		std::string get() override {

			// start with normal texture coordinate stored within the global variable "uv"
			return get("uv");

		}


		std::string get(std::string uv) {

			// TODO: order? tiling->mod or mod->tiling?

			// apply the attached UV modifier [if any] to the original [untiled] uv-coordinate
			if (modUV) {uv = "(" + uv + " + " + modUV->getOffset(FRAG_VERTEX_UV) + ")";}

			// apply texture tiling [if any]
			if (tile != Vec2(1,1)) {uv = "(" + uv + " * vec2(" + std::to_string(tile.x) + "," + std::to_string(tile.y) + "))";}

			// build output
			const std::string base = "texture(" + texID + ", " + uv + ")";
			std::string res;
			if		("rgba" == mode)	{res = base;}

			else if (mode.empty())		{res = base;}

			else if	("rgb" == mode)		{res = "vec4("+base+".rgb, 1.0)";}

			else if ("grey" == mode)	{res = "vec4(vec3("+base+".a), 1.0)";}		// gray images are stored as alpha-only textures

			// use fast multiply-add and swizzling to build the output
			else if ("map" == mode)	{

				std::string tex = base + ".";
				std::string mul = "vec4(";
				std::string add = "vec4(";

				if (r[0] == '.') {tex += r[1];  mul += "1.0,"; add += "0.0,";} else {tex += "r"; mul += "0.0,"; add += r+",";}
				if (g[0] == '.') {tex += g[1];  mul += "1.0,"; add += "0.0,";} else {tex += "r"; mul += "0.0,"; add += g+",";}
				if (b[0] == '.') {tex += b[1];  mul += "1.0,"; add += "0.0,";} else {tex += "r"; mul += "0.0,"; add += b+",";}
				if (a[0] == '.') {tex += a[1];  mul += "1.0" ; add += "0.0" ;} else {tex += "r"; mul += "0.0" ; add += a;}

				mul += ")";
				add += ")";

				res = tex + " * " + mul + " + " + add;

			}


			// color modifier?
			if (modColor) {
				res = modColor->getModified(res);
			}

			// done
			return res;

		}

	};

}

#endif // SOURCETEXTURE_H
