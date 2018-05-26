#ifndef SCENE_LIGHTING_H
#define SCENE_LIGHTING_H

#include "../gl/UBO.h"
#include "Light.h"
#include "../material/ProgramVariables.h"

/**
 * describes the scene's lighting
 */
class Lighting {

private:

	/** UBO to be uploaded into the GPU [per Frame] */
	//LightAttributes __attribute__((packed)) lightData[MAX_LIGHTS];
#pragma pack(push,1)
	struct {
		AmbientAttributes ambientData;
		LightAttributes lightData[MAX_LIGHTS];
	} lighting;
#pragma pack(pop)


	/** wrapper to modify the UBO-data */
	Ambient ambient;

	/** wrapper to modify the UBO-data */
	Light lights[MAX_LIGHTS];


	/** UBO upload helper */
	UBO ubo;

public:

	/** ctor */
	Lighting() {

		// init wrappers
		ambient = Ambient(&(lighting.ambientData));
		for (int i = 0; i < MAX_LIGHTS; ++i) {
			lights[i] = Light(&(lighting.lightData[i]));
		}

		// default
		ambient.setColor(1,1,1);
		ambient.setImpact(0.2);

	}

	UBO& getUBO() {
		return ubo;
	}

	Light& getLight(const int idx) {
		return lights[idx];
	}

	Ambient& getAmbient() {
		return ambient;
	}

	/** upload current config to the GPU [usually once per rendered frame] */
	void upload() {
		const size_t sizeLights = sizeof(lighting);
		ubo.upload((uint8_t*)&lighting, sizeLights);
	}

};

#endif // SCENE_LIGHTING_H
