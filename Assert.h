#ifndef K3DE_ASSERT_H
#define K3DE_ASSERT_H

#include "Exception.h"
#include <sstream>
#include <cmath>

// for GTEST Testcases
#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

/**
 * this file provides assertion methods that may be enabled to trace
 * coding errors efficiently during debug or disabled to speed up the
 * code execution for the release build
 *
 * compile with -DWITH_ASSERTIONS
 */
namespace Assert {


	template <typename STR> static inline void doThrow(const STR err) {
		#ifdef WITH_ASSERTIONS
			std::string str = "in: ";
			str += __PRETTY_FUNCTION__;
			str += " error: ";
			str += err;
			throw Exception(err);
				#else
					(void) err;
		#endif
	}

	template <typename T, typename STR> static inline void equal(const T v1, const T v2, const STR err) {
		if (v1 != v2) {doThrow(err);}
	}

	template <typename T, typename STR> static inline void notEqual(const T v1, const T v2, const STR err) {
		if (v1 == v2) {doThrow(err);}
	}

	template <typename T, typename STR> static inline void isTrue(const T v, const STR err) {
		if (!v) {doThrow(err);}
	}

	template <typename T, typename STR> static inline void isFalse(const T v, const STR err) {
		if (v) {doThrow(err);}
	}

	template <typename T, typename STR> static inline void isNull(const T v, const STR err) {
		if (v != nullptr) {doThrow(err);}
	}

	template <typename T, typename STR> static inline void isNotNull(const T v, const STR err) {
		if (v == nullptr) {doThrow(err);}
	}

	template <typename T, typename STR> static inline void isNotNaN(const T v, const STR err) {
		if (v != v) {doThrow(err);}
	}

	template <typename T, typename STR> static inline void isNot0(const T v, const STR err) {
		if (v == 0) {doThrow(err);}
	}

	template <typename T, typename STR> static inline void isNear(const T v1, const T v2, const T delta, const STR err) {
		if (std::abs(v1-v2) > delta) {
			std::stringstream ss; ss << "\nexpected " << v1 << " +/- " << delta << " but is " << v2 << "\n";
			doThrow(err+ss.str());
		}
	}

	template <typename T, typename STR> static inline void isBetween(const T v, const T min, const T max, const STR err) {
		if (v < min || v > max) {
			std::stringstream ss; ss << "\n[" << min << ":" << max << "] but is " << v << "\n";
			doThrow(err+ss.str());
		}
	}

}

namespace StaticAssert {

	// yes and no have a different size so sizeof(yes) != sizeof(no)
	using yes = uint32_t;
	using no = uint64_t;

	template<class BaseClass, class SubClass> struct isXbaseOfY {
		static yes test(const BaseClass&);									// will be chosen if X is base of Y
		static no  test(...);												// will be chosen otherwise
		static const SubClass& helper();
		static const bool value = sizeof(test(helper())) == sizeof(yes);	// true if test(const B&) was chosen
	};

	template <typename A, typename B> static inline void AinheritsB() {
		static_assert(isXbaseOfY<B, A>::value, "A does not inherit from B!");
	}


}

#endif // ASSERT_H
