#ifndef TEXTUREDEBUG_H
#define TEXTUREDEBUG_H

#include "Texture.h"
#include "../import/Image.h"

//#define WITH_DEBUG_PREVIEW

#ifdef WITH_DEBUG_PREVIEW
#include <opencv2/opencv.hpp>
#endif

class TextureDebug {

public:

	static void show(const std::string& name, const Texture* tex, GLuint format) {

		#ifdef WITH_DEBUG_PREVIEW

			const int w = tex->getWidth();
			const int h = tex->getHeight();

			// tmp destination
			std::vector<uint8_t> tmp( w*h*4 );

			// fetch the texture
			glBindTexture(GL_TEXTURE_2D, tex->getID());
			Error::assertOK();

			int dataType;
			int matDataType;

			switch(format) {
				case GL_DEPTH_COMPONENT: dataType = GL_FLOAT; matDataType = CV_32FC1; break;
				case GL_RGB: dataType = GL_BYTE, matDataType = CV_8UC3; break;
			}

			Error::assertOK();

			glGetTexImage(GL_TEXTURE_2D, 0, format, dataType, tmp.data());
			cv::Mat mat(h, w, matDataType, tmp.data());
			cv::namedWindow(name);
			cv::imshow(name, mat);
			cv::waitKey(1);

		#endif

	}

};

#endif // TEXTUREDEBUG_H
