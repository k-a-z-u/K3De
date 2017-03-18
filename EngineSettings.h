#ifndef ENGINESETTINGS_H
#define ENGINESETTINGS_H

#include <string>
#include "scene/ScreenSize.h"

struct EngineSettings {
    
    /** currently used size [depends on the window's size] */
    //ScreenSize screen;
    int windowWidth = 640;
    int windowHeight = 480;

	int antiAliasing = 0;

	std::string title = "Engine";

};

#endif // ENGINESETTINGS_H
