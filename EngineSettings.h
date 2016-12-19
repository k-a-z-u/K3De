#ifndef ENGINESETTINGS_H
#define ENGINESETTINGS_H

#include <string>
#include "scene/ScreenSize.h"

struct EngineSettings {

	ScreenSize screen;

	int antiAliasing = 0;

	std::string title = "Engine";

};

#endif // ENGINESETTINGS_H
