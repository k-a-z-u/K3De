R"=====(

out mat3 normalRotMat;

/**
 * get a matrix, that rotates vec3(0,0,1) into the provided vec3.
 * this matrix is used, to rotate a normal map
 * (used for bump-mapping, blue-color = (0,0,1)
 * into the coordinate-system of the vertex' normal.
 */
mat3 getNormalRotationMatrix(const vec3 normal) {
	return getRotationMatrix( vec3(0,0,1), normalize(normal) );
}

)====="
