#ifndef ENGINESETTINGS_H
#define ENGINESETTINGS_H

#include <string>

struct EngineSettings {

	struct {
		int width = 640;
		int height = 480;
	} screen;

	int antiAliasing = 0;

	std::string title = "Engine";

};

#endif // ENGINESETTINGS_H
