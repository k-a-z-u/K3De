#ifndef PROVLIGHTHELPER_H
#define PROVLIGHTHELPER_H

class ProvLightHelper {

public:

	static std::string isLightEnabled(const std::string& idx) {
		return "(lights.light[" + idx + "].flags & 1) != 0";
	}

	static std::string isPointLight(const std::string& idx) {
		return "lights.light[" + idx + "].type == 0";
	}

	static std::string isDirectionalLight(const std::string& idx) {
		return "lights.light[" + idx + "].type == 1";
	}

	static std::string lightCastsShadows(const std::string& idx) {
		return "(lights.light[" + idx + "].flags & 2) != 0";
	}

	/** get the light's position (if this is a point light) */
	static std::string getLightPos(const std::string& idx) {
		return "lights.light[" + idx + "].pos.xyz";
	}

	/** get the light's direction (if this is a directional light) */
	static std::string getLightDirection(const std::string& idx) {
		return "lights.light[" + idx + "].pos.xyz";
	}

	static std::string getLightColor(const std::string& idx) {
		return "lights.light[" + idx + "].color.rgb";
	}

	static std::string getAmbientColor() {
		return "lights.ambient.color.rgb";
	}

};

#endif // PROVLIGHTHELPER_H
