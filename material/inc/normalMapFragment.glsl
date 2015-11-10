R"=====(
//vec3 getNormalFromBumMap(const sampler2D sampler, const vec2 uv) {
//	return normalize(texture(sampler, uv).rgb - 0.5);
//}

in mat3 normalRotMat;

//float getNormalImpact(const vec3 bumpMapNormal) {
//	return clamp( dot(vec3(0,0,1), bumpMapNormal), 0, 1);
//}
)====="
