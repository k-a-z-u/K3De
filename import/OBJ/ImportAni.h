#ifndef IMPORTANI_H
#define IMPORTANI_H

namespace OBJ {

	/**
	 * .obj files do not support animation by themselves.
	 * however, many 3D editors support exporting one .obj per frame.
	 * and, depending on the export, some parts stay the same among several .obj files,
	 * like the list of triangle indices
	 */
	class ImportAni {


	};

}

#endif // IMPORTANI_H
