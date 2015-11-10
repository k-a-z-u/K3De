R"=====(
// TODO: switch to clipping via plane equation
uniform vec2 clipY = vec2(-9999,+9999);

void doClip() {
	bool clip = (vertex_M.y < clipY[0]) || (vertex_M.y > clipY[1]);
	if (clip) {discard;}
}
)====="
