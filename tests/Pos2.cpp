
#ifdef WITH_TESTS

#include "Tests.h"
#include "../2D/Pos2.h"

TEST(Pos2, pos) {

	MetricX x(15, Unit::PIXEL);
	MetricY y(10, Unit::PIXEL);
	MetricX w(30, Unit::PIXEL);
	MetricY h(20, Unit::PIXEL);
	ScreenSize ss(240, 100);

	Pos2 pos1(x, y, AnchorX::LEFT, AnchorY::UPPER);
	Vec2 vec1 = pos1.toUpperLeft(ss, w, h);
	ASSERT_NEAR(0.0625, vec1.x, 0.000001);
	ASSERT_NEAR(0.1000, vec1.y, 0.000001);

	Pos2 pos2(x, y, AnchorX::RIGHT, AnchorY::LOWER);
	Vec2 vec2 = pos2.toUpperLeft(ss, w, h);
	ASSERT_NEAR(0.1875, vec2.x, 0.000001);
	ASSERT_NEAR(0.3000, vec2.y, 0.000001);

	Pos2 pos3(x, y, AnchorX::CENTER, AnchorY::CENTER);
	Vec2 vec3 = pos3.toUpperLeft(ss, w, h);
	ASSERT_NEAR(0.125, vec3.x, 0.000001);
	ASSERT_NEAR(0.200, vec3.y, 0.000001);


}

#endif
