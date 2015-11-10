R"=====(
/** represents one light. TODO: switch to vec3 for pos/color! */
struct Light {
	float x,y,z,r,g,b;
};

/** uniform containing the light-setup */
uniform Lights {
	Light[8] light;
	int cnt;
} lights;

/** get the number of to-be-used lights */
int getNumLights() {
	return lights.cnt;
}

/** get the position of the idx-th light */
vec3 getLightPos(const int idx) {
	return vec3(lights.light[idx].x, lights.light[idx].y, lights.light[idx].z);
}

/** get the color of the idx-th ligth */
vec3 getLightColor(const int idx) {
	return vec3(lights.light[idx].r, lights.light[idx].g, lights.light[idx].b);
}
)====="
