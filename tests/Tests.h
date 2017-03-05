#ifndef TESTS_H
#define TESTS_H

#ifdef WITH_TESTS

#define MAX_LIGHTS 8
#define TODO(msg)

#include <gtest/gtest.h>

#include "../data/Resource.h"

#define ASSERT_BYTE_EQ(arr1, arr2, len) {\
		for (int i = 0; i < len; ++i) {\
			ASSERT_EQ((arr1)[i], (arr2)[i]);\
		}\
}

template<typename T> void ASSERT_ARR_EQ(T* arr1, T* arr2, const int len) {
	for (int i = 0; i < len; ++i) {
		ASSERT_EQ(arr1[i], arr2[i]);
	}
}
template<typename T> void ASSERT_ARR_NEAR(T* arr1, T* arr2, const int len, const float eps) {
	for (int i = 0; i < len; ++i) {
		ASSERT_NEAR(arr1[i], arr2[i], eps);
	}
}

//static inline std::string getDataFile(const std::string& name) {
//	return "/mnt/vm/workspace/IRGame/K3De/tests/data/" + name;
//}
static inline Resource getDataFile(const std::string& name) {
	return Resource("/mnt/vm/workspace/IRGame/K3De/tests/data/" + name);
}



#endif


#endif // TESTS_H
