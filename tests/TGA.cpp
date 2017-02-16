#ifdef WITH_TESTS

#include "Tests.h"

#include "../import/TGA.h"
#include "../import/ImageFactory.h"

TEST(TGA, load) {

	const Resource res("/tmp/wall_512_5_05.tga");

	TGA tga;
	Image img = tga.get(res);

	ImageFactory::saveJPG("/tmp/test.jpg", img);

}

#endif
