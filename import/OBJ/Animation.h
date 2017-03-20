#ifndef ANIMATION_H
#define ANIMATION_H

#include "../../mesh/MeshVertex.h"
#include <vector>

namespace OBJ {

	/**
	 * a frame is given by its vertices and their normal/tangent/texture
	 * thus each frame must have the same number of vertices, AND,
	 * for the triangles to work correctly, they must have the same order!
	 */
	struct Frame {
		std::vector<AttrVertexNormalTangentTexture> vertices;
	};

	/**
	 * parsed animation data
	 * several frames and a given FPS-rate
	 */
	struct Animation {

		int fps;

		/** all frames that belong to the animation */
		std::vector<Frame> frames;

		/** get the idx-th frame */
		const Frame& getFrame(const int idx) {return frames[idx];}

		/** number of frames this animation has */
		int getNumFrames() const {return frames.size();}

	};

}

#endif // ANIMATION_H
