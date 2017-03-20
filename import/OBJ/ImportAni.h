#ifndef IMPORTANI_H
#define IMPORTANI_H

#include "ImportMesh.h"
#include "Animation.h"

#include "../../data/Resource.h"

namespace OBJ {

	/**
	 * .obj files do not support animation by themselves.
	 * however, many 3D editors support exporting one .obj per frame.
	 * and, depending on the export, some parts stay the same among several .obj files,
	 * like the list of triangle indices
	 */
	class ImportAni {

	public:

		static Animation load(const std::vector<Resource>& meshes) {
			return _load(meshes);
		}

		static Animation load(const std::vector<Data>& meshes) {
			return _load(meshes);
		}

	private:

		template <typename Src> static Animation _load(const Src& meshes) {

			Animation ani;

			// parse each mesh [= one frame]
			for (const auto& src : meshes) {

				// NOTE:
				// neither normalizeation nor centering is available as this would be PER FRAME! and thus kill everything!
				OBJ::Mesh mesh = ImportMesh::load(src, false, false);
				Frame frame;
				frame.vertices = mesh.getIndexedMeshVerticesWithTangent();
				ani.frames.push_back(frame);
				ani.fps = 25;

			}

			// done
			return ani;

		}


		// for non-static non-threadsafe access

	private:

		Animation res;

	public:

		/** ctor */
		ImportAni() {
			res.fps = 25;
		}

		/** add a new "frame" to the animation */
		void add(const Resource& mesh) {
			_add(mesh);
		}

		/** add a new "frame" to the animation */
		void add(const Data& mesh) {
			_add(mesh);
		}

		/** get the constructed animation */
		const Animation& get() const {
			return res;
		}

	private:

		template <typename T> void _add(const T& src) {

			// NOTE
			// neither normalizeation nor centering is available as this would be PER FRAME! and thus kill everything!
			OBJ::Mesh mesh = ImportMesh::load(src, false, false);
			Frame frame;
			frame.vertices = mesh.getIndexedMeshVerticesWithTangent();
			res.frames.push_back(frame);

		}


	};

}

#endif // IMPORTANI_H
