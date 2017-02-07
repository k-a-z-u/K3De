#ifndef SCENE_LIGHTING_H
#define SCENE_LIGHTING_H

#include "../gl/UBO.h"
#include "Light.h"

/**
 * describes the scene's lighting
 */
class Lighting {

private:

	#define MAX_LIGHTS		8

	/** UBO to be uploaded into the GPU [per Frame] */
	LightAttributes __attribute__((packed)) lightData[MAX_LIGHTS];

	/** wrapper to modify the UBO-data */
	Light lights[MAX_LIGHTS];

	/** UBO upload helper */
	UBO ubo;

public:

	/** ctor */
	Lighting() {

		// init wrappers
		for (int i = 0; i < MAX_LIGHTS; ++i) {
			lights[i] = Light(&lightData[i]);
		}

	}

	UBO& getUBO() {
		return ubo;
	}

	Light& getLight(const int idx) {
		return lights[idx];
	}

	/** upload current config to the GPU [usually once per rendered frame] */
	void upload() {
		const size_t sizeLights = sizeof(lightData);
		ubo.upload((uint8_t*)&lightData, sizeLights);
	}

};

#endif // SCENE_LIGHTING_H
