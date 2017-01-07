#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <cstdint>
#include <cmath>

#include "Pixel.h"

enum class ImageFormat {
	IMAGE_GREY,
	IMAGE_GREY_ALPHA,
	IMAGE_RGB,
	IMAGE_RGBA,
};



class Image {

private:

	friend class ImageFactory;

	int w;
	int h;
	ImageFormat format;
	std::vector<uint8_t> data;

public:

	/** empty ctor */
	Image() {;}

	/** empty ctor */
	Image(const int w, const int h, const ImageFormat fmt) : w(w), h(h), format(fmt) {
		reformat(w,h,fmt);
	}

	/** get the underlying data vector */
	const std::vector<uint8_t>& getData() const {
		return data;
	}

	/** modifyable data pointer */
	uint8_t* getDataPtr() {return data.data();}

	/** un-modifyable data pointer */
	const uint8_t* getDataPtr() const {return data.data();}

	int getWidth() const {return w;}

	int getHeight() const {return h;}

	/** get the RGB value at the given pixel */
	RGBi getRGBi(const int x, const int y) const {
		switch (format) {
			case ImageFormat::IMAGE_RGB:	{const int idx = (x+y*w)*3;	return RGBi(data[idx+0], data[idx+1], data[idx+2]);}
			case ImageFormat::IMAGE_RGBA:	{const int idx = (x+y*w)*4;	return RGBi(data[idx+0], data[idx+1], data[idx+2]);}
			default:						throw "not yet implemented";
		}
	}

	void setRGBi(const int x, const int y, const RGBi rgb) {
		const int idx = (x+y*w)*3;
		data[idx+0] = rgb.r;
		data[idx+1] = rgb.g;
		data[idx+2] = rgb.b;
	}

	/** get the interpolated RGB value at the given pixel */
	RGBi getRGBi_int(const float x, const float y) {

		const RGBf x1y1 = getRGBi(std::floor(x), std::floor(y)).toRGBf();
		const RGBf x2y1 = getRGBi( std::ceil(x), std::floor(y)).toRGBf();
		const RGBf x1y2 = getRGBi(std::floor(x),  std::ceil(y)).toRGBf();
		const RGBf x2y2 = getRGBi( std::ceil(x),  std::ceil(y)).toRGBf();

		const float sx = x - std::floor(x);
		const float sy = y - std::floor(y);
		const RGBf xy1 = (x1y1 * (1-sx)) + (x2y1 * sx);
		const RGBf xy2 = (x1y2 * (1-sx)) + (x2y2 * sx);
		const RGBf xy = (xy1 * (1-sy)) + (xy2 * sy);

		return xy.toRGBi();

	}

	/** get the GREY value at the given pixel */
	int getGrey(const int x, const int y) const {
		switch (format) {
			case ImageFormat::IMAGE_GREY:	{const int idx = (x+y*w)*1;		return data[idx];}
			case ImageFormat::IMAGE_RGB:	{const int idx = (x+y*w)*3;		return data[idx];}
			case ImageFormat::IMAGE_RGBA:	{const int idx = (x+y*w)*4;		return data[idx];}
			default:						throw "not yet implemented";
		}
	}



	/** convert this image into a grey one */
	Image toGrey() {
		Image out;	out.reformat(w, h, ImageFormat::IMAGE_GREY);
		for (int y = 0; y < h; ++y) {
			for (int x = 0; x < w; ++x) {
				out.data.push_back(getGrey(x,y));
			}
		}
		return out;
	}

	/** get a scaled version of this image */
	Image scale(const float v) {
		int nw = (int)(w * v);
		int nh = (int)(h * v);
		Image out;	out.reformat(nw, nh, format);
		for (int y = 0; y < nh; ++y) {
			for (int x = 0; x < nw; ++x) {
				const float fx = x / v;
				const float fy = y / v;
				switch (format) {
					case ImageFormat::IMAGE_RGB:	out.setRGBi(x, y, getRGBi_int(fx, fy)); break;
					default:						throw "not yet implemented";
				}
			}
		}
		return out;
	}

	ImageFormat getFormat() const {return format;}


protected:

	/** set the image's size/format and allocate enough memory to hold the corresponding data */
	void reformat(const int w, const int h, const ImageFormat fmt) {
		this->w = w;
		this->h = h;
		this->format = fmt;
		switch (fmt) {
			case ImageFormat::IMAGE_RGB:		this->data.reserve(w*h*3); break;
			case ImageFormat::IMAGE_RGBA:		this->data.reserve(w*h*4); break;
			case ImageFormat::IMAGE_GREY:		this->data.reserve(w*h*1); break;
			case ImageFormat::IMAGE_GREY_ALPHA:	this->data.reserve(w*h*2); break;
			default:							throw "not yet implemented";
		}
	}

};

#endif // IMAGE_H
