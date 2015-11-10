R"=====(
in vec4 shadowCoord;

uniform sampler2D texShadowMap;
const float shadowSize = 0.01;
const float shadowDarkness = 0.1;

/** is the current fragment shadowed? */
float isShadowed(const vec3 sc) {

	// smallest distance from this given (x,y) to the light
	float nearest = texture(texShadowMap, sc.st).r;

	// current distance to the light
	float curDist = sc.z;

	// shadowed?
	return (nearest < curDist) ? (shadowDarkness) : (1.0);

}

/** is the current fragment shadowed? */
float shadowDistance(const vec3 sc) {

	// smallest distance from this given (x,y) to the light
	float nearest = texture(texShadowMap, sc.st).r;

	// current distance to the light
	float curDist = sc.z;

	// shadowed?
	return curDist-nearest;

}


float chebyshevUpperBound(float distance, const vec2 moments) {

	// Surface is fully lit. as the current fragment is before the light occluder
	if (distance < moments.x) {return 1.0 ;}

	// The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
	// How likely this pixel is to be lit (p_max)
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance, 0.000001f);

	float d = distance - moments.x;
	float p_max = variance / (variance + d*d);

	return p_max;
}

float shadowed() {

//	// if we are behind the light, no shadowing is possible (prevents artifacts)
//	if (shadowCoord.w <= 0.0) {return 1.0;}

//	// copy
//	vec3 sc = shadowCoord.xyz;

//	// add some clearance to prevent almost-equal-z artifcats
//	sc.z -= 0.0005;

//	// scale the vector (why?!)
//	sc = (sc / shadowCoord.w);

//	// "smooth" the shadow
//	float s = shadowSize / shadowCoord.w;
//	//float s = (0.001 + shadowDistance(sc)) * 0.1;
//	float sum = 0;
//	sum += isShadowed(sc + vec3( 0, 0, 0));
//	sum += isShadowed(sc + vec3(+s,+s, 0));
//	sum += isShadowed(sc + vec3(-s,-s, 0));
//	sum += isShadowed(sc + vec3(-s,+s, 0));
//	sum += isShadowed(sc + vec3(+s,-s, 0));
//	return sum/5;




	// if we are behind the light, no shadowing is possible (prevents artifacts)
	if (shadowCoord.w < 0.0) {return 1.0;}

	// copy
	// sc is within range [0,1] as shadowCoord was created using a biased matrix
	vec3 sc = shadowCoord.xyz / shadowCoord.w;

//	// get the moments (distance and squared-distance to the light) from the shadow map
//	vec2 moments = texture(texShadowMap, sc.xy).rg;

//	return chebyshevUpperBound(sc.z, moments);


	float expOccluderDistanceToLight = texture(texShadowMap, sc.xy).r;
	if (expOccluderDistanceToLight == 0) {return 1.0;}

	float curDistToLight = sc.z;// * 0.5 + 0.5;
	//float x = exp( 900 * (expOccluderDistanceToLight-curDistToLight) );
	float x = expOccluderDistanceToLight * exp(-40 * curDistToLight);
	x = clamp(x, 0, 1);
	//x = min(x, 1);
	//x = pow(x, 10);
	return x;

//	float expOccluderDist = texture(texShadowMap, sc.xy).r;
//	if (expOccluderDist == 1) {return 1.0;}

//	float viewDepth = sc.z;// + 0.0005;// - DEPTH_BIAS;

//	float visibility = expOccluderDist / exp(-5 * viewDepth);
//	return visibility;


}
)====="
