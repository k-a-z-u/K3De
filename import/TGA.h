#ifndef TGA_H
#define TGA_H

#include "Image.h"
#include "../data/Resource.h"
#include "../data/ResourceFactory.h"

class TGA {

	/** 1 byte image type */
	enum ImageType : uint8_t {
		NO_IMAGE_DATA = 0,
		UNCOMPRESSED_COLOR_MAPPED = 1,
		UNCOMPRESSED_TRUE_COLOR = 2,
		UNCOMPRESSED_BW = 3,
		RLE_COLOR_MAPPED = 9,
		RLE_TRUE_COLOR = 10,
		RLE_BW = 11,
	};

	struct ColorMap {
		uint16_t idx1;
		uint16_t numEntries;
		uint8_t bitsPerPixel;
	} __attribute__((__packed__));

	struct ImageSpecs {

		uint16_t originX;
		uint16_t originY;

		uint16_t width;
		uint16_t height;

		uint8_t bitsPerPixel;
		uint8_t imageDesc;			// bits 3-0 give the alpha channel depth, bits 5-4 give direction

	} __attribute__((__packed__));

	struct Header {

		uint8_t imageIDLength;
		uint8_t colorMapType;
		ImageType imageType;
		ColorMap colorMap;
		ImageSpecs specs;


	} __attribute__((__packed__));



public:

	Image get(const Resource& res) {

		Data data = ResourceFactory::get().get(res);

		Header* header = (Header*) data.get();

		// sanity checks
		if (header->specs.originX != 0)						{throw Exception("x-origin not yet supported");}
		if (header->specs.originY != 0)						{throw Exception("y-origin not yet supported");}
		if (header->imageType != UNCOMPRESSED_TRUE_COLOR)	{throw Exception("currently only uncompressed true-color images are supported");}
		if (header->colorMap.numEntries != 0)				{throw Exception("color-map must be empty!");}
		if (header->specs.imageDesc != 0)					{throw Exception("Image Descriptor must currently be zero!");}

		// after the header follows the image-ID-Field and the [empty] palette, then -> data!
		uint8_t* rawStart = data.get() + (sizeof(Header) + header->imageIDLength + 0 + 1);		// why +1?

		Image img;

		if (header->specs.bitsPerPixel == 24) {

			img = Image(header->specs.width, header->specs.height, ImageFormat::IMAGE_RGB);
			memcpy(img.getDataPtr(), rawStart, img.getWidth()*img.getHeight()*3);

		} else if (header->specs.bitsPerPixel == 32) {

			img = Image(header->specs.width, header->specs.height, ImageFormat::IMAGE_RGBA);
			memcpy(img.getDataPtr(), rawStart, img.getWidth()*img.getHeight()*4);

		} else {
			throw Exception("unsupported bit depth: " + std::to_string(header->specs.bitsPerPixel));
		}

		return img;

	}


};

#endif // TGA_H
