#ifndef AMBIENTTEXTURE_H
#define AMBIENTTEXTURE_H

#include "../../textures/ITexture.h"
#include "../../math/Math.h"

//#include "AmbientTextureMode.h"

enum class TextureType {

	UNSET,

	COLOR,
	DISPLACEMENT,
	ALPHA,

};

struct TypedTexture {

	TextureType type;

	ITexture* texture;

	Vec2 tiling;


	/** empty ctor */
	TypedTexture() : type(TextureType::UNSET), texture(nullptr), tiling() {;}

	/** ctor */
	TypedTexture(TextureType type, ITexture* texture, Vec2 tiling) : type(type), texture(texture), tiling(tiling) {;}

};

struct ColorTexture : public TypedTexture {

	/** empty ctor */
	ColorTexture() : TypedTexture() {;}

	/** ctor */
	ColorTexture(ITexture* texture, Vec2 tiling) : TypedTexture(TextureType::COLOR, texture, tiling) {;}

};

struct DisplacementTexture : public TypedTexture {

	float strength;

	/** empty ctor */
	DisplacementTexture() : TypedTexture(), strength(0) {;}

	/** ctor */
	DisplacementTexture(ITexture* texture, Vec2 tiling, float strength) : TypedTexture(TextureType::COLOR, texture, tiling), strength(strength) {;}

};

#endif // AMBIENTTEXTURE_H
