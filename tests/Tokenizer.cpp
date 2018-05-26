#ifdef WITH_TESTS

#include "Tests.h"
#include "../misc/Tokenizer.h"

TEST(Tokenizer, Test1) {
	std::string s = "123/456/789";
	Tokenizer t(s);
	std::string t1 = t.getToken('/', false);
	std::string t2 = t.getToken('/', false);
	std::string t3 = t.getToken('/', false);
	ASSERT_EQ("123", t1);
	ASSERT_EQ("456", t2);
	ASSERT_EQ("789", t3);
}

TEST(Tokenizer, Test2) {
	std::string s = "123//456";
	Tokenizer t(s);
	std::string t1 = t.getToken('/', false);
	std::string t2 = t.getToken('/', false);
	std::string t3 = t.getToken('/', false);
	ASSERT_EQ("123", t1);
	ASSERT_EQ("", t2);
	ASSERT_EQ("456", t3);
}

TEST(Tokenizer, Test3) {
	std::string s = "v  123   456    7.8  9.0 10";
	Tokenizer t(s);
	ASSERT_EQ("v", t.getToken(' '));
	ASSERT_EQ("123", t.getToken(' '));
	ASSERT_EQ("456", t.getToken(' '));
	ASSERT_EQ("7.8", t.getToken(' '));
	ASSERT_EQ("9.0", t.getToken(' '));
	ASSERT_EQ("10", t.getToken(' '));
}

TEST(Tokenizer, Test4) {
	std::string s = "f 1/1/1 2/2/2 3/3/3 \r";
	Tokenizer t(s, '\r');
	ASSERT_EQ("f", t.getToken(' '));
	ASSERT_EQ("1/1/1", t.getToken(' '));
	ASSERT_EQ("2/2/2", t.getToken(' '));
	ASSERT_EQ("3/3/3", t.getToken(' '));
	ASSERT_FALSE(t.hasNext());
}

#endif
