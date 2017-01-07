#ifdef WITH_TESTS

#include "Tests.h"
#include "../math/Math.h"


TEST(Matrix, mulSpeed) {

	static constexpr int NUM_MATS = 256;
	static constexpr int NUM_RUNS = 1024*1024*10;

	std::vector<Mat4> mats;
	mats.resize(NUM_MATS);

	for (int i = 0; i < NUM_RUNS; ++i) {

		const int idx1 = (i+1337) % NUM_MATS;
		const int idx2 = (i+13) % NUM_MATS;
		const int idx3 = (i+482) % NUM_MATS;
		mats[idx1] = mats[idx2] * mats[idx3];

	}

}

TEST(Matrix, mul) {



	float a[16] = {
		1.000,  2.000,  3.000,  4.000,
		5.000,  6.000,  7.000,  8.000,
		9.000, 10.000, 11.000, 12.000,
	   13.000, 14.000, 15.000, 16.000,
	};

	float b[16] = {
		21.000, 22.000, 23.000, 25.000,
		19.000, 10.000, 11.000, 33.000,
		20.000, 12.000, 29.000, 13.000,
		15.000, 19.000, 13.000, 10.000,
	};

	float ab[16] = {
		179.000,  154.000,  184.000,  170.000,
		479.000,  406.000,  488.000,  494.000,
		779.000,  658.000,  792.000,  818.000,
	   1079.000,  910.000, 1096.000, 1142.000,
	};

	Mat4 matA = Mat4::fromRawRowMajor(a);
	Mat4 matB = Mat4::fromRawRowMajor(b);
	Mat4 matAB= Mat4::fromRawRowMajor(ab);

	Mat4 res = matA * matB;

	ASSERT_EQ(matAB, res);


}


#endif
