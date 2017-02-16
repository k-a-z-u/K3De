#ifdef WITH_TESTS

#include "Tests.h"

#include "../data/Archive.h"

TEST(_7Z, load) {

	Archive archive("/tmp/test.7z");

	Data d = archive.get("Main/Debug.h");
	std::cout << d.size() << std::endl;


}

#endif
