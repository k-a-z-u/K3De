#ifndef SHADERBINARY_H
#define SHADERBINARY_H

#include "../gl/gl.h"

#include "../data/Data.h"

struct ProgramBinary {
	Data data;
	GLenum format;
};

struct ShaderBinary {
	Data data;
	GLenum format;
};

#endif // SHADERBINARY_H
