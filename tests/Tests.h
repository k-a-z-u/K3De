#ifndef TESTS_H
#define TESTS_H

#ifdef WITH_TESTS

#include <gtest/gtest.h>

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



#endif


#endif // TESTS_H
