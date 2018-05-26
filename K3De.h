#ifndef K3DE_H
#define K3DE_H

#if defined(__GNUC__)
	#define TODO(txt)
#else
	#define TODO(txt)
#endif

#include "scene/Scene.h"
#include "Engine.h"



#include "EngineImpl.h"
#include "scene/SceneImpl.h"
#include "material/MaterialFactoryImpl.h"

#include "material3/Material3.h"
#include "material3/Material3Compiler.h"
#include "material3/MaterialFactory3Impl.h"

#include "world/TerrainFactory.h"


#endif // K3DE_H
