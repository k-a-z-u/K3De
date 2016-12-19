
#ifdef WITH_TESTS

#include "Tests.h"
#include "../3D/Path.h"

TEST(Path, round) {
	Path p;
	p.add(Vec3(-1, 0,-1));
	p.add(Vec3(+1, 0,-1));
	p.add(Vec3(+1, 0,+1));
	p.add(Vec3(-1, 0,+1));
	p.add(Vec3(-1, 0,-1));
	p.round(0.2);
	p.round(0.2);

	std::cout << "TODO: path.round()" << std::endl;

//	for (const Vec3& v : p.getPoints()) {
//		std::cout << v.x << " " << v.y << " " << v.z << std::endl;
//	}

}

#endif
