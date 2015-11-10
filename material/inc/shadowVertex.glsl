R"=====(

out vec4 shadowCoord;

uniform mat4 PVshadow;

void calcShadowParams() {
	shadowCoord = PVshadow * vec4(vertex_M, 1);
}

)====="
