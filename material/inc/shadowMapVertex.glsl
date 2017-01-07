R"=====(

#version 300 es
precision mediump float;

layout(location = 0) in vec3 vertex;

//uniform mat4 M;
//uniform mat4 V;
//uniform mat4 P;
uniform mat4 PVM;

// the position on screen (within the unit-cube)
//out vec4 vertex_P;

void main(){

    /** the projection of input vertex */
    //vertex_P = P*V*M * vec4(vertex, 1.0);
    //vertex_P = PVM * vec4(vertex, 1.0);

    //gl_Position = vertex_P;

    gl_Position = PVM * vec4(vertex, 1.0);

}

)====="
