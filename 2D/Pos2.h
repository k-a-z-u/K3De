#ifndef POS2_H
#define POS2_H

#include "../math/Math.h"
#include "../scene/ScreenSize.h"


enum class Unit {
	PERCENT,
	PIXEL,
};

/** describes a value including its unit */
struct Metric {

	float value;
	Unit unit;

	/** empty ctor */
	Metric() : value(0), unit(Unit::PERCENT) {;}

	/** ctor */
	Metric(const float value, const Unit unit) : value(value), unit(unit) {;}

};

struct MetricX : public Metric {

	/** empty ctor */
	MetricX() : Metric() {;}

	/** ctor */
	MetricX(const float value, const Unit unit) : Metric(value, unit) {;}

	/** convert to percent of screen width */
	float toPercent(const ScreenSize& screen) const {
		switch(unit) {
			case Unit::PIXEL:		return value / screen.width;
			case Unit::PERCENT:		return value;
			default: throw "coding error";
		}
	}

};

struct MetricY : public Metric {

	/** empty ctor */
	MetricY() : Metric() {;}

	/** ctor */
	MetricY(const float value, const Unit unit) : Metric(value, unit) {;}

	/** convert to percent of screen height */
	float toPercent(const ScreenSize& screen) const {
		switch(unit) {
			case Unit::PIXEL:		return value / screen.height;
			case Unit::PERCENT:		return value;
			default: throw "coding error";
		}
	}

};

/** describes the margin of an object */
struct Margin2 {

	Metric left;
	Metric right;
	Metric top;
	Metric bottom;

	/** ctor */
	Margin2() {
		;
	}

	/** ctor */
	Margin2(const Metric left, const Metric right, const Metric top, const Metric bottom) :
		left(left), right(right), top(top), bottom(bottom) {
		;
	}

};

enum class AnchorX {
	LEFT,
	CENTER,
	RIGHT,
};

enum class AnchorY {
	UPPER,
	CENTER,
	LOWER,
};

struct Pos2 {

	MetricX x;
	MetricY y;

	AnchorX ax;
	AnchorY ay;

	/** empty ctor */
	Pos2() : ax(AnchorX::LEFT), ay(AnchorY::LOWER) {
		;
	}

	/** ctor */
	Pos2(const MetricX x, const MetricY y, const AnchorX ax, const AnchorY ay) :
	    x(x), y(y), ax(ax), ay(ay) {
		;
	}

	Vec2 toUpperLeft(const ScreenSize& screen, const MetricX w, const MetricY h) const {

		const float pw = w.toPercent(screen);
		const float ph = h.toPercent(screen);

		const float px = x.toPercent(screen);
		const float py = y.toPercent(screen);

		float ox;
		float oy;

		switch(ax) {
			case AnchorX::LEFT:		ox = px; break;
			case AnchorX::CENTER:	ox = px - pw * 0.5f; break;
			case AnchorX::RIGHT:	ox = px - pw; break;
			default:				throw "error";
		}

		switch (ay) {
			case AnchorY::UPPER:	oy = py; break;
			case AnchorY::CENTER:	oy = py - ph * 0.5f; break;
			case AnchorY::LOWER:	oy = py - ph; break;
			default:				throw "error";
		}

		return Vec2(ox, oy);

	}

};

struct Size2 {

	MetricX x;
	MetricY y;

	/** empty ctor */
	Size2() {;}

	/** ctor */
	Size2(const MetricX x, const MetricY y) : x(x), y(y) {;}

	Vec2 toVec(const ScreenSize& screen) const {
		return Vec2(x.toPercent(screen), y.toPercent(screen));
	}

};




///**
// * describes a 2D position using various units
// */
//class Unit2 {

//private:

//	float x;
//	float y;
//	float ratio;

//	enum class Unit {
//		PERCENT,
//		PIXEL,
//	} unit;



//	/** empty ctor */
//	Unit2() {
//		;
//	}

//	/** ctor */
//	Unit2(const float x, const float y, const Unit unit) : x(x), y(y), unit(unit) {
//		;
//	}

//public:

//	/** mapping to scene coordinates requires scene details [screen-size, aspect-ratio, ..] */
//	Vec2 toVec(Scene* scene) {
//		switch(unit) {
//			case Unit::PIXEL:
//				return Vec2(x / scene->getScreenSize().width, y / scene->getScreenSize().height);
//			case Unit::PERCENT:
//				if (x != x) {return Vec2(y * ratio * scene->getScreenSize().getAspect(), y);}
//				if (y != y) {return Vec2(x, x / ratio / scene->getScreenSize().getAspect());}
//							{return Vec2(x, y * scene->getScreenSize().getAspect());}
//			default:
//				throw Exception("coding error");
//		}
//	}

//	/** 2D position using pixel coordinates */
//	Unit2 px(const float x, const float y) {
//		return Pos2(x, y, Unit::PIXEL);
//	}

//	/** percentage depends on the screen's aspect ratio. thus the x value and the ratio (x/y) is provided */
//	Unit2 percentX(const float x, const float ratio) {
//		Pos2 pos;
//		pos.x = x;
//		pos.y = NAN;
//		pos.ratio = ratio;
//		pos.unit = Unit::PERCENT;
//		return pos;
//	}

//	Unit2 percent(const float x, const float y) {
//		Pos2 pos;
//		pos.x = x;
//		pos.y = y;
//		pos.unit = Unit::PERCENT;
//		return pos;
//	}

//};

//class Anchor2 : public Unit2 {

//	Unit2 value;

//	enum class Type {
//		LOWER_LEFT,
//		CENTER_LEFT,
//		UPPER_LEFT,
//		CENTER,
//	} type;

//	static Anchor2 lowerLeftPercent(const float x, const float y) {
//		Anchor2 a;
//		a.type = Type::LOWER_LEFT;
//		a.value = Unit2::percent(x, y);
//		return a;
//	}

//	static Anchor2 centerLeftPercent(const float x, const float y) {
//		Anchor2 a;
//		a.type = Type::CENTER_LEFT;
//		a.value = Unit2::percent(x, y);
//		return a;
//	}

//	static Anchor2 upperLeftPercent(const float x, const float y) {
//		Anchor2 a;
//		a.type = Type::UPPER_LEFT;
//		a.value = Unit2::percent(x, y);
//		return a;
//	}

//	static Anchor2 l(const float x, const float y) {
//		Anchor2 a;
//		a.type = Type::CENTER_LEFT;
//		a.value = Unit2::percent(x, y);
//		return a;
//	}

//	static Anchor2 lowerLeftX(const float x, const float ratio) {
//		Anchor2 a;
//		a.type = Type::LOWER_LEFT;
//		a.value = Unit2::percentX(x, ratio);
//		return a;
//	}

//	static Anchor2 lowerLeftX(const float x, const float ratio) {
//		Anchor2 a;
//		a.type = Type::LOWER_LEFT;
//		a.value = Unit2::percentX(x, ratio);
//		return a;
//	}

//};

//class Size2 {

//	Unit2 value;

//};


#endif // POS2_H
