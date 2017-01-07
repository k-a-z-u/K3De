#ifndef RECT_H
#define RECT_H

#include "Pos2.h"

struct Rect {

	const ScreenSize* screen;
	Pos2 anchor;
	Size2 size;

	/** ctor */
	Rect(const ScreenSize* screen, const Pos2 anchor, const Size2 size) : screen(screen), anchor(anchor), size(size) {;}

	/** ctor */
	Rect() : screen(nullptr), anchor(), size() {
		;
	}

	/** get the upper-left coordinate */
	Vec2 getUpperLeft() const {
		return anchor.toUpperLeft(*screen, size.x, size.y);
	}

	/** get the lower-right coordinate */
	Vec2 getLowerRight() const {
		return getUpperLeft() + size.toVec(*screen);
	}


public:

	static Rect fromLowerLeft(const Vec2 pos, const Vec2 size) {
		Rect r;
		r.anchor = Pos2(MetricX(pos.x, Unit::PERCENT), MetricY(pos.y, Unit::PERCENT), AnchorX::LEFT, AnchorY::LOWER);
		r.size = Size2(MetricX(size.x, Unit::PERCENT), MetricY(size.y, Unit::PERCENT));
		return r;
	}

	static Rect fromCenterLeft(const Vec2 pos, const Vec2 size) {
		Rect r;
		r.anchor = Pos2(MetricX(pos.x, Unit::PERCENT), MetricY(pos.y, Unit::PERCENT), AnchorX::LEFT, AnchorY::CENTER);
		r.size = Size2(MetricX(size.x, Unit::PERCENT), MetricY(size.y, Unit::PERCENT));
		return r;
	}

	static Rect fromUpperLeft(const Vec2 pos, const Vec2 size) {
		Rect r;
		r.anchor = Pos2(MetricX(pos.x, Unit::PERCENT), MetricY(pos.y, Unit::PERCENT), AnchorX::LEFT, AnchorY::UPPER);
		r.size = Size2(MetricX(size.x, Unit::PERCENT), MetricY(size.y, Unit::PERCENT));
		return r;
	}

	static Rect fromCenter(const Vec2 pos, const Vec2 size) {
		Rect r;
		r.anchor = Pos2(MetricX(pos.x, Unit::PERCENT), MetricY(pos.y, Unit::PERCENT), AnchorX::CENTER, AnchorY::CENTER);
		r.size = Size2(MetricX(size.x, Unit::PERCENT), MetricY(size.y, Unit::PERCENT));
		return r;
	}


};

///** rectangle for 2D sprites */
//struct Rect {

//	float left;
//	float top;
//	float right;
//	float bottom;

//	/** ctor */
//	Rect(const float left, const float top, const float right, const float bottom) :
//		left(left), top(top), right(right), bottom(bottom) {
//		;
//	}

//	/** ctor */
//	Rect() : left(0), top(0), right(0), bottom(0) {
//		;
//	}

//	/** get the upper-left coordinate */
//	Vec2 getUpperLeft() const {return Vec2(left, top);}

//	/** get the lower-right coordinate */
//	Vec2 getLowerRight() const {return Vec2(right, bottom);}




//	static Rect fromLowerLeft(const Vec2 pos, const Vec2 size) {
//		Rect r;
//		r.left = pos.x;
//		r.bottom = pos.y;
//		r.right = r.left + size.x;
//		r.top = r.bottom - size.y;
//		return r;
//	}

//	static Rect fromCenterLeft(const Vec2 pos, const Vec2 size) {
//		Rect r;
//		r.left = pos.x;
//		r.right = r.left + size.x;
//		r.top = pos.y - size.y/2;
//		r.bottom = pos.y + size.y/2;
//		return r;
//	}

//	static Rect fromUpperLeft(const Vec2 pos, const Vec2 size) {
//		Rect r;
//		r.left = pos.x;
//		r.top = pos.y;
//		r.right = r.left + size.x;
//		r.bottom = r.top + size.y;
//		return r;
//	}

//	static Rect fromCenter(const Vec2 pos, const Vec2 size) {
//		Rect r;
//		r.left = pos.x - size.x/2;
//		r.right = pos.x + size.x/2;
//		r.top = pos.y - size.y/2;
//		r.bottom = pos.y + size.y/2;
//		return r;
//	}

//};

#endif // RECT_H
