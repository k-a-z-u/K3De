R"=====(

#version 300 es
precision mediump float;


void main(){

	// just depth output
	gl_FragDepth = gl_FragCoord.z;
	//color.r =  gl_FragCoord.z;

}

)====="
