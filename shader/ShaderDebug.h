#ifndef SHADERDEBUG_H
#define SHADERDEBUG_H

#include "Shader.h"
#include <unordered_map>

// http://stackoverflow.com/questions/19894839/opengl-uniform-array-of-structures#19918602

class ShaderDebug {




private:

	std::unordered_map<GLenum, std::string> map;

	void build() {

		map[	GL_INVALID_ENUM] =                             "invalid";
		map[	GL_FLOAT] =                                    "float";
		map[	GL_FLOAT_VEC2] =                               "vec2";
		map[	GL_FLOAT_VEC3] =                               "vec3";
		map[	GL_FLOAT_VEC4] =                               "vec4";
		map[	GL_DOUBLE] =                                   "double";
		map[	GL_DOUBLE_VEC2] =                              "dvec2";
		map[	GL_DOUBLE_VEC3] =                              "dvec3";
		map[	GL_DOUBLE_VEC4] =                              "dvec4";
		map[	GL_INT] =                                      "int";
		map[	GL_INT_VEC2] =                                 "ivec2";
		map[	GL_INT_VEC3] =                                 "ivec3";
		map[	GL_INT_VEC4] =                                 "ivec4";
		map[	GL_UNSIGNED_INT] =                             "unsigned int";
		map[	GL_UNSIGNED_INT_VEC2] =                        "uvec2";
		map[	GL_UNSIGNED_INT_VEC3] =                        "uvec3";
		map[	GL_UNSIGNED_INT_VEC4] =                        "uvec4";
		map[	GL_BOOL] =                                     "bool";
		map[	GL_BOOL_VEC2] =                                "bvec2";
		map[	GL_BOOL_VEC3] =                                "bvec3";
		map[	GL_BOOL_VEC4] =                                "bvec4";
		map[	GL_FLOAT_MAT2] =                               "mat2";
		map[	GL_FLOAT_MAT3] =                               "mat3";
		map[	GL_FLOAT_MAT4] =                               "mat4";
		map[	GL_FLOAT_MAT2x3] =                             "mat2x3";
		map[	GL_FLOAT_MAT2x4] =                             "mat2x4";
		map[	GL_FLOAT_MAT3x2] =                             "mat3x2";
		map[	GL_FLOAT_MAT3x4] =                             "mat3x4";
		map[	GL_FLOAT_MAT4x2] =                             "mat4x2";
		map[	GL_FLOAT_MAT4x3] =                             "mat4x3";
		map[	GL_DOUBLE_MAT2] =                              "dmat2";
		map[	GL_DOUBLE_MAT3] =                              "dmat3";
		map[	GL_DOUBLE_MAT4] =                              "dmat4";
		map[	GL_DOUBLE_MAT2x3] =                            "dmat2x3";
		map[	GL_DOUBLE_MAT2x4] =                            "dmat2x4";
		map[	GL_DOUBLE_MAT3x2] =                            "dmat3x2";
		map[	GL_DOUBLE_MAT3x4] =                            "dmat3x4";
		map[	GL_DOUBLE_MAT4x2] =                            "dmat4x2";
		map[	GL_DOUBLE_MAT4x3] =                            "dmat4x3";
		map[	GL_SAMPLER_1D] =                               "sampler1D";
		map[	GL_SAMPLER_2D] =                               "sampler2D";
		map[	GL_SAMPLER_3D] =                               "sampler3D";
		map[	GL_SAMPLER_CUBE] =                             "samplerCube";
		map[	GL_SAMPLER_1D_SHADOW] =                        "sampler1DShadow";
		map[	GL_SAMPLER_2D_SHADOW] =                        "sampler2DShadow";
		map[	GL_SAMPLER_1D_ARRAY] =                         "sampler1DArray";
		map[	GL_SAMPLER_2D_ARRAY] =                         "sampler2DArray";
		map[	GL_SAMPLER_1D_ARRAY_SHADOW] =                  "sampler1DArrayShadow";
		map[	GL_SAMPLER_2D_ARRAY_SHADOW] =                  "sampler2DArrayShadow";
		map[	GL_SAMPLER_2D_MULTISAMPLE] =                   "sampler2DMS";
		map[	GL_SAMPLER_2D_MULTISAMPLE_ARRAY] =             "sampler2DMSArray";
		map[	GL_SAMPLER_CUBE_SHADOW] =                      "samplerCubeShadow";
		map[	GL_SAMPLER_BUFFER] =                           "samplerBuffer";
		map[	GL_SAMPLER_2D_RECT] =                          "sampler2DRect";
		map[	GL_SAMPLER_2D_RECT_SHADOW] =                   "sampler2DRectShadow";
		map[	GL_INT_SAMPLER_1D] =                           "isampler1D";
		map[	GL_INT_SAMPLER_2D] =                           "isampler2D";
		map[	GL_INT_SAMPLER_3D] =                           "isampler3D";
		map[	GL_INT_SAMPLER_CUBE] =                         "isamplerCube";
		map[	GL_INT_SAMPLER_1D_ARRAY] =                     "isampler1DArray";
		map[	GL_INT_SAMPLER_2D_ARRAY] =                     "isampler2DArray";
		map[	GL_INT_SAMPLER_2D_MULTISAMPLE] =               "isampler2DMS";
		map[	GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] =         "isampler2DMSArray";
		map[	GL_INT_SAMPLER_BUFFER] =                       "isamplerBuffer";
		map[	GL_INT_SAMPLER_2D_RECT] =                      "isampler2DRect";
		map[	GL_UNSIGNED_INT_SAMPLER_1D] =                  "usampler1D";
		map[	GL_UNSIGNED_INT_SAMPLER_2D] =                  "usampler2D";
		map[	GL_UNSIGNED_INT_SAMPLER_3D] =                  "usampler3D";
		map[	GL_UNSIGNED_INT_SAMPLER_CUBE] =                "usamplerCube";
		map[	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY] =            "usampler2DArray";
		map[	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY] =            "usampler2DArray";
		map[	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE] =      "usampler2DMS";
		map[	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY] ="usampler2DMSArray";
		map[	GL_UNSIGNED_INT_SAMPLER_BUFFER] =              "usamplerBuffer";
		map[	GL_UNSIGNED_INT_SAMPLER_2D_RECT] =             "usampler2DRect";
		map[	GL_IMAGE_1D] =                                 "image1D";
		map[	GL_IMAGE_2D] =                                 "image2D";
		map[	GL_IMAGE_3D] =                                 "image3D";
		map[	GL_IMAGE_2D_RECT] =                            "image2DRect";
		map[	GL_IMAGE_CUBE] =                               "imageCube";
		map[	GL_IMAGE_BUFFER] =                             "imageBuffer";
		map[	GL_IMAGE_1D_ARRAY] =                           "image1DArray";
		map[	GL_IMAGE_2D_ARRAY] =                           "image2DArray";
		map[	GL_IMAGE_2D_MULTISAMPLE] =                     "image2DMS";
		map[	GL_IMAGE_2D_MULTISAMPLE_ARRAY] =               "image2DMSArray";
		map[	GL_INT_IMAGE_1D] =                             "iimage1D";
		map[	GL_INT_IMAGE_2D] =                             "iimage2D";
		map[	GL_INT_IMAGE_3D] =                             "iimage3D";
		map[	GL_INT_IMAGE_2D_RECT] =                        "iimage2DRect";
		map[	GL_INT_IMAGE_CUBE] =                           "iimageCube";
		map[	GL_INT_IMAGE_BUFFER] =                         "iimageBuffer";
		map[	GL_INT_IMAGE_1D_ARRAY] =                       "iimage1DArray";
		map[	GL_INT_IMAGE_2D_ARRAY] =                       "iimage2DArray";
		map[	GL_INT_IMAGE_2D_MULTISAMPLE] =                 "iimage2DMS";
		map[	GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY] =           "iimage2DMSArray";
		map[	GL_UNSIGNED_INT_IMAGE_1D] =                    "uimage1D";
		map[	GL_UNSIGNED_INT_IMAGE_2D] =                    "uimage2D";
		map[	GL_UNSIGNED_INT_IMAGE_3D] =                    "uimage3D";
		map[	GL_UNSIGNED_INT_IMAGE_2D_RECT] =               "uimage2DRect";
		map[	GL_UNSIGNED_INT_IMAGE_CUBE] =                  "uimageCube";
		map[	GL_UNSIGNED_INT_IMAGE_BUFFER] =                "uimageBuffer";
		map[	GL_UNSIGNED_INT_IMAGE_1D_ARRAY] =              "uimage1DArray";
		map[	GL_UNSIGNED_INT_IMAGE_2D_ARRAY] =              "uimage2DArray";
		map[	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE] =        "uimage2DMS";
		map[	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY] =  "uimage2DMSArray";
		map[	GL_UNSIGNED_INT_ATOMIC_COUNTER] =              "atomic_uint";

	}


public:

	void dumpUniforms (Shader* shader) {

		build();
		const GLuint program = shader->getProgram();

	  GLint uniform_count;
	  glGetProgramiv (program, GL_ACTIVE_UNIFORMS, &uniform_count);

	  GLchar name [256];

	  for (GLint i = 0; i < uniform_count; i++) {
		memset (name, '\0', 256);
		GLint  size;
		GLenum type;

		glGetActiveUniform (program, i, 255, NULL, &size, &type, name);

		GLint location = glGetUniformLocation (program, name);

//		for (int j = 0; j < sizeof (type_set) / sizeof (glsl_type_set); j++) {
//		  if (type_set [j].type != type)
//			continue;

//		  const char* type_name = type_set [j].name;
		const char* type_name = map[type].c_str();

		  if (size > 1) {
			printf ( "Uniform %d (loc=%d):\t%20s %-20s <Size: %d>\n", i, location, type_name, name, size );
		  } else {
			printf ( "Uniform %d (loc=%d):\t%20s %-20s\n", i, location, type_name, name );
		  }

		 // break;
//		}

		if (i == (uniform_count - 1))
		  printf ("\n");
	  }
	}

private:

};

#endif // SHADERDEBUG_H
