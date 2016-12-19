
#ifdef WITH_TESTS

#include "Tests.h"
#include "../math/Math.h"

TEST(Math, MatrixIdentity) {
	Mat4 mat(1.0f);
	ASSERT_EQ(1, mat.get(0,0));	ASSERT_EQ(0, mat.get(1,0));	ASSERT_EQ(0, mat.get(2,0));	ASSERT_EQ(0, mat.get(3,0));
	ASSERT_EQ(0, mat.get(0,1));	ASSERT_EQ(1, mat.get(1,1));	ASSERT_EQ(0, mat.get(2,1));	ASSERT_EQ(0, mat.get(3,1));
	ASSERT_EQ(0, mat.get(0,2));	ASSERT_EQ(0, mat.get(1,2));	ASSERT_EQ(1, mat.get(2,2));	ASSERT_EQ(0, mat.get(3,2));
	ASSERT_EQ(0, mat.get(0,3));	ASSERT_EQ(0, mat.get(1,3));	ASSERT_EQ(0, mat.get(2,3));	ASSERT_EQ(1, mat.get(3,3));
}

TEST(Math, MatrixMul1) {
	Mat4 mat1(1.0f);
	Mat4 mat2(1.0f);
	Mat4 mat = mat1*mat2;
	ASSERT_EQ(1, mat.get(0,0));	ASSERT_EQ(0, mat.get(1,0));	ASSERT_EQ(0, mat.get(2,0));	ASSERT_EQ(0, mat.get(3,0));
	ASSERT_EQ(0, mat.get(0,1));	ASSERT_EQ(1, mat.get(1,1));	ASSERT_EQ(0, mat.get(2,1));	ASSERT_EQ(0, mat.get(3,1));
	ASSERT_EQ(0, mat.get(0,2));	ASSERT_EQ(0, mat.get(1,2));	ASSERT_EQ(1, mat.get(2,2));	ASSERT_EQ(0, mat.get(3,2));
	ASSERT_EQ(0, mat.get(0,3));	ASSERT_EQ(0, mat.get(1,3));	ASSERT_EQ(0, mat.get(2,3));	ASSERT_EQ(1, mat.get(3,3));
}

TEST(Math, MatrixMul2) {
	float data1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	float data2[16] = {4, 3, 2, 1, 8, 7, 6, 5, 12, 11, 10, 9, 16, 15, 14, 13};
	float data3[16] = {120, 110, 100, 90, 280, 254, 228, 202, 440, 398, 356, 314, 600, 542, 484, 426};
	Mat4 mat1 = Mat4::fromRawRowMajor(data1);
	Mat4 mat2 = Mat4::fromRawRowMajor(data2);
	Mat4 mat = mat1*mat2;
	ASSERT_BYTE_EQ(data3, mat.data(), 16);
}

TEST(Math, MatrixTranspose) {
	float data1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	float data2[16] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
	Mat4 mat1 = Mat4::fromRawRowMajor(data1);	 mat1.transpose();
	ASSERT_BYTE_EQ(data2, mat1.data(), 16);
}

TEST(Math, MatrixVectorMul) {
	Vec3 vec3(1,2,3);
	Vec4 vec4(1,2,3,4);
	Mat3 mat3(1.0f);
	Mat4 mat4(1.0f);
	Vec3 vec3b = mat3 * vec3;
	Vec4 vec4b = mat4 * vec4;
	ASSERT_EQ(vec3, vec3b);
	ASSERT_EQ(vec4, vec4b);
}

TEST(Math, MatrixVectorMul2) {
	Vec3 vec3(1,2,3);
	const float data[] = {1,2,3, 4,5,6, 7,8,9};
	Mat3 mat3 = Mat3::fromRawRowMajor(data);
	ASSERT_EQ(Vec3(14,32,50), mat3*vec3);
}

//#include <ctime>
//TEST(Math, BenchmarkMatrix) {
//	clock_t begin = clock();
//	const int runs = 10240;
//	const int cnt = 1024;
//	Mat4 mat[cnt];
//	for (int j = 0; j < runs; ++j) {
//		int idx = rand() % cnt;
//		for (int i = 0; i < (cnt-1); ++i) {
//			mat[idx] = mat[i+1] * mat[i];
//		}
//	}
//	clock_t end = clock();
//	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//	std::cout << elapsed_secs << " secs" << std::endl;
//	int i;
//}

//TEST(Math, BenchmarkMatrixVector) {
//	clock_t begin = clock();
//	const int runs = 10240;
//	const int cnt = 1024;
//	Mat4 mat[cnt];
//	Vec4 vec[cnt];
//	for (int j = 0; j < runs; ++j) {
//		int idx = rand() % cnt;
//		for (int i = 0; i < (cnt-1); ++i) {
//			vec[idx] = mat[i+1] * vec[i];
//		}
//	}
//	clock_t end = clock();
//	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//	std::cout << elapsed_secs << " secs" << std::endl;
//	int i;
//}

TEST(Math, VectorEqual) {
	Vector<3> vec1(0);
	Vector<3> vec2(0);
	ASSERT_EQ(vec1, vec2);
	Vector<4> vec3(1);
	Vector<4> vec4(1);
	ASSERT_EQ(vec3, vec4);
	Vector<2> vec5(0);
	Vector<2> vec6(1);
	ASSERT_NE(vec5, vec6);
}

TEST(Math, VectorSub) {

	Vector<3> vec1(+1);
	Vector<3> vec2(-1);
	ASSERT_EQ(-vec1, vec2);

	Vector<4> vec3(1);
	Vector<4> vec4(0);
	ASSERT_EQ(vec3-vec3, vec4);

	vec3 -= vec3;
	ASSERT_EQ(vec3, vec4);

}

#ifdef WITH_TESTS_OLD
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
TEST(Math, CameraLookAt) {

	glm::mat4 mat1 = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
	Mat4 mat2 = Math::camLookAt(Vec3(0,0,1), Vec3(0,0,0), Vec3(0,1,0));
	const float* data1 = &mat1[0][0];
	const float* data2 = mat2.data();
	ASSERT_ARR_NEAR(data1, data2, 16, 0.0001);

	glm::mat4 mat3 = glm::lookAt(glm::vec3(2,2,2), glm::vec3(1,1,1), glm::vec3(0,1,0));
	Mat4 mat4 = Math::camLookAt(Vec3(2,2,2), Vec3(1,1,1), Vec3(0,1,0));
	const float* data3 = &mat3[0][0];
	const float* data4 = mat4.data();
	ASSERT_ARR_NEAR(data3, data4, 16, 0.0001);

}

TEST(Math, CameraPerspective) {

	glm::mat4 mat1 = glm::perspective(45.0, 4.0/3.0, 0.1, 10.0);
	Mat4 mat2 = Math::camPerspective(45.0, 4.0/3.0, 0.1, 10.0);
	const float* data1 = &mat1[0][0];
	const float* data2 = mat2.data();
	ASSERT_ARR_NEAR(data1, data2, 16, 0.0001);

}

#endif

TEST(Math, VectorLength) {
	Vec3 v1(1,2,3);
	ASSERT_NEAR(3.7414, Math::length(v1), 0.001);
	Vec3 v2(0,0,-1);
	ASSERT_NEAR(1.0, Math::length(v2), 0.001);
}

TEST(Math, VecNormalize) {
	Vec3 v1(1,2,3);
	Vec3 v2 = Math::normalize(v1);
	ASSERT_NEAR(0.26726, v2.x, 0.001);
	ASSERT_NEAR(0.53452, v2.y, 0.001);
	ASSERT_NEAR(0.80178, v2.z, 0.001);
	Vec3 v3(0,0,-1);
	Vec3 v4 = Math::normalize(v3);
	ASSERT_NEAR(+0, v4.x, 0.001);
	ASSERT_NEAR(+0, v4.y, 0.001);
	ASSERT_NEAR(-1, v4.z, 0.001);
}

TEST(Math, VecSub) {
	Vec3 v1(0,0,0);
	Vec3 v2(0,0,1);
	Vec3 v3 = v1-v2;
	ASSERT_NEAR(+0, v3.x, 0.001);
	ASSERT_NEAR(+0, v3.y, 0.001);
	ASSERT_NEAR(-1, v3.z, 0.001);
}


TEST(Math, VecDiv) {
	Vec3 v1(3,6,9);
	Vec3 v2 = v1/3;
	float res[3] = {1, 2, 3};
	ASSERT_BYTE_EQ(res, v2.data, 3);
}

TEST(Math, VecCross) {

	Vec3 v1(1,2,3);
	Vec3 v2(4,6,8);
	Vec3 v3 = Math::cross(v1, v2);
	float res[3] = {-2, 4, -2};
	ASSERT_ARR_NEAR(res, v3.data, 3, 0.0001);

	v1 = Vec3(19, 89, 1);
	v2 = Vec3(9, 99, 8);
	v3 = Math::cross(v1,v2);
	float res2[3] = {613, -143, 1080};
	ASSERT_ARR_NEAR(res2, v3.data, 3, 0.0001);

}

#endif
