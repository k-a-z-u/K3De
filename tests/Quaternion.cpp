
#ifdef WITH_TESTS

#include "Tests.h"
#include "../math/Math.h"

TEST(Quaternion, rotate) {

	// axis that points to the right: rotate by 90 degree
	const Quaternion q = Quaternion::fromAxisAngle(Vec3(1,0,0), -90.0 / 180.0 * M_PI);

	// vector that points backwards
	const Vec3 pos(0,0,1);

	// vector that should point upwards (90 degree rotation)
	const Vec3 out = q.rotate(pos);

	ASSERT_NEAR(out.x, 0, 0.0001);
	ASSERT_NEAR(out.y, 1, 0.0001);
	ASSERT_NEAR(out.z, 0, 0.0001);

}

TEST(Quaternion, inverse) {

	// axis that points to the right: rotate by 90 degree
	const Quaternion q = Quaternion::fromAxisAngle(Vec3(1,0,0), -90.0 / 180.0 * M_PI);

	// vector that points backwards
	const Vec3 v0(0,0,1);

	// vector that should point upwards (90 degree rotation)
	const Vec3 v1 = q.rotate(v0);

	// vector that should point backwards
	const Vec3 v2 = q.inverse().rotate(v1);

	ASSERT_NEAR(v0.x, v2.x, 0.0001);
	ASSERT_NEAR(v0.y, v2.y, 0.0001);
	ASSERT_NEAR(v0.z, v2.z, 0.0001);

}

TEST(Quaternion, inverse2) {

	const Quaternion q = Quaternion::fromAxisAngle(Vec3(1,2,3), -77.0 / 180.0 * M_PI);

	const Vec3 v0(3,1,2);

	const Vec3 v1 = q.rotate(v0);

	const Vec3 v2 = q.inverse().rotate(v1);

	ASSERT_NEAR(v0.x, v2.x, 0.0001);
	ASSERT_NEAR(v0.y, v2.y, 0.0001);
	ASSERT_NEAR(v0.z, v2.z, 0.0001);

}

#endif
