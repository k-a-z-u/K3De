#ifndef PIXEL_H
#define PIXEL_H

struct RGBi;
struct RGBf;

struct RGBi {

	uint8_t r;
	uint8_t g;
	uint8_t b;

	RGBi() {;}
	RGBi(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b) {;}

	RGBf toRGBf() const;

};

struct RGBf {

	float r;
	float g;
	float b;

	RGBf() {;}
	RGBf(const float r, const float g, const float b) : r(r), g(g), b(b) {;}

	RGBf operator + (const RGBf& o) const {return RGBf(r+o.r, g+o.g, b+o.b);}
	RGBf operator * (const float v) const {return RGBf(r*v, g*v, b*v);}

	RGBi toRGBi() const;
};

RGBf RGBi::toRGBf() const {return RGBf(r/255.0f, g/255.0f, b/255.0f);}
RGBi RGBf::toRGBi() const {return RGBi(r*255, g*255, b*255);}

#endif // PIXEL_H
