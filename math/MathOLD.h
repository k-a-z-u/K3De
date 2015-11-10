#ifndef MATHOLD_H
#define MATHOLD_H

// perspective
//		const float top = near * std::tan(toRadians(fov/2.0f));
////		const float bottom = -top;
//		const float right = top * aspect;
////		const float left = -right;
////		const float mat[16] = {
////			(2*near)/(right-left),	0,						(right+left)/(right-left),	0,
////			0,						(2*near)/(top-bottom),	(top+bottom)/(top-bottom),	0,
////			0,						0,						-(far+near)/(far-near),		-(2*far*near)/(far-near),
////			0,						0,						-1,							0
////		};
//		const float mat[16] = {
//			(near)/(right),	0,				0,							0,
//			0,				(near)/(top),	0,							0,
//			0,				0,				-(far+near)/(far-near),		-(2*far*near)/(far-near),
//			0,				0,				-1,							0
//		};
//		return glm::transpose( glm::make_mat4(mat) );
		//return glm::perspective(fov, aspect, near, far);


#endif // MATHOLD_H
