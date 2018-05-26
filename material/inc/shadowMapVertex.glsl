R"=====(

{VERSION}
precision mediump float;

layout(location = 0) in vec3 vertex;

//uniform mat4 M;
//uniform mat4 V;
//uniform mat4 P;
uniform mat4 PVM;

// the position on screen (within the unit-cube)
//out vec4 vertex_P;

vec4 worldToTexture(vec4 src) {
	//src.w = 1.0;
	vec4 tmp = normalize(src/src.w);
	float u = (atan(tmp.z, tmp.x) / (3.1415926535));
	float v = asin(tmp.y) / (3.1415926535 / 2.0);
	float z = src.z / src.w / 100.0;	// TODO dynamic 100. also within ProvLightShadow
	return vec4(u, v, z, 1.0);
}
uniform mat4 M;
uniform mat4 V;

void main(){

    /** the projection of input vertex */
    //vertex_P = P*V*M * vec4(vertex, 1.0);
    //vertex_P = PVM * vec4(vertex, 1.0);

    //gl_Position = vertex_P;

	//gl_Position = PVM * vec4(vertex, 1.0);
	gl_Position = worldToTexture(V * M * vec4(vertex, 1.0));	// test

}

)====="
