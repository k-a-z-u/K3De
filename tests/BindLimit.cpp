#ifdef WITH_TESTS

#include "Tests.h"

#include <set>

class BindLimitMany {
public:
	#include "../misc/BindOnceMany.h"
};

TEST(BindLimit, many) {

	BindLimitMany m;

	// ADD

	ASSERT_FALSE(m.isBound(10));
	m.setBound(10);
	ASSERT_TRUE(m.isBound(10));

	ASSERT_FALSE(m.isBound(20));
	m.setBound(20);
	ASSERT_TRUE(m.isBound(10));
	ASSERT_TRUE(m.isBound(20));

	ASSERT_FALSE(m.isBound(11));
	m.setBound(11);
	ASSERT_TRUE(m.isBound(10));
	ASSERT_TRUE(m.isBound(11));
	ASSERT_TRUE(m.isBound(20));

	// REMOVE

	m.setUnbound(10);
	ASSERT_FALSE(m.isBound(10));
	ASSERT_TRUE(m.isBound(11));
	ASSERT_TRUE(m.isBound(20));

	m.setUnbound(20);
	ASSERT_FALSE(m.isBound(10));
	ASSERT_TRUE(m.isBound(11));
	ASSERT_FALSE(m.isBound(20));

	m.setUnbound(11);
	ASSERT_FALSE(m.isBound(10));
	ASSERT_FALSE(m.isBound(11));
	ASSERT_FALSE(m.isBound(20));

}

#endif
