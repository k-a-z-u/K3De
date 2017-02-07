
#ifdef WITH_TESTS

#include "Tests.h"

#include "../math/Vector.h"
#include "../math/Quaternion.h"

#include "../Engine.h"

#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"

#include "../gl/VAO.h"
#include "../gl/UBO.h"

#include "../gl/VBO.h"
#include "../gl/VBOArray.h"
#include "../gl/VBOArrayIndex.h"

#include "../mesh/IMesh.h"
#include "../mesh/IndexedMesh.h"
#include "../mesh/InstanceMesh.h"
#include "../mesh/MeshVertex.h"
#include "../mesh/MD5Mesh.h"

#include "../import/ObjImport.h"
#include "../import/MD5/ImportAni.h"
#include "../import/MD5/ImportMesh.h"

#include "../material/MaterialFactory.h"
#include "../material/MaterialFactoryImpl.h"

#include "../scene/Scene.h"
#include "../scene/WaterRenderer.h"

#include "../world/Water.h"
#include "../world/Terrain.h"
#include "../world/Skybox.h"

#include "../2D/Text.h"
#include "../2D/Text2.h"
#include "../2D/Sprite.h"

#include "../3D/Plane.h"
#include "../3D/Box.h"


#endif
