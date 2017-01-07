R"=====(
///** get a matrix, that rotates v1 into v2. both vectors MUST be normalized! */
//mat3 getRotationMatrix(const vec3 v1, const vec3 v2) {

//	float a = acos(dot(v1,v2));
//	vec3 u = normalize(cross(v2,v1));

//	vec3 A = vec3(cos(a) + u.x*u.x * (1.0f-cos(a)),		u.x*u.y*(1.0f-cos(a))-u.z*sin(a),		u.x*u.z*(1.0f-cos(a))+u.y*sin(a)	);
//	vec3 B = vec3(u.y*u.x*(1.0f-cos(a))+u.z*sin(a),		cos(a)+u.y*u.y*(1.0f-cos(a)),			u.y*u.z*(1.0f-cos(a))-u.x*sin(a)	);
//	vec3 C = vec3(u.z*u.x*(1.0f-cos(a))-u.y*sin(a),		u.z*u.y*(1.0f-cos(a))+u.x*sin(a),		cos(a)+u.z*u.z*(1.0f-cos(a))		);

//	return mat3(A,B,C);

//}
)====="
