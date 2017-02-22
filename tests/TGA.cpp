#ifdef WITH_TESTS

#include "Tests.h"

#include "../import/image/TGA.h"
#include "../import/ImageFactory.h"

TEST(TGA, load) {

	const Resource res(getDataFile("img1.tga"));

	Image img = ImageFactory::load(res);
	ImageFactory::save("/tmp/test.tga", img);

}

#endif
